package main

import (
	"regexp"
	"fmt"
	"strconv"
)

type ttype int

const (
	tokIf = iota
	tokIfdef
	tokIfndef
	tokDefed
	tokDef
	tokUndef
	tokElse
	tokEndif
	tokInc

	tokStr
	tokId
	tokNum

	tokNot
	tokAnd
	tokOr
	tokOparen
	tokCparen
	tokEq
	tokNe
	tokGe
	tokLe
	tokLt
	tokGt
)

var (
	lineno = 0;

	tokstr = map[ttype]string{
		tokIf:     "#if",
		tokIfdef:  "#ifdef",
		tokIfndef: "#ifndef",
		tokDefed:  "defined",
		tokDef:    "#define",
		tokUndef:  "#undef",
		tokEndif:  "#endif",
		tokInc:    "#include",
		tokStr:    "<string>",
		tokId:     "<identifier>",
		tokNum:    "<number>",
		tokNot:    "!",
		tokAnd:    "&&",
		tokOr:     "||",
		tokOparen: "(",
		tokCparen: ")",
		tokEq:     "==",
		tokNe:     "!=",
		tokGe:     ">=",
		tokLe:     "<=",
		tokLt:     "<",
		tokGt:     ">",
	}

	directives = map[string]ttype {
		"if":      tokIf,
		"ifdef":   tokIfdef,
		"ifndef":  tokIfndef,
		"define":  tokDef,
		"undef":   tokUndef,
		"endif":   tokEndif,
		"else":    tokElse,
		"include": tokInc,
	}

	keywords = map[string]ttype {
		"defined":  tokDefed,
	}

	opers = map[string]ttype{
		"(":  tokOparen,
		")":  tokCparen,
		"&&": tokAnd,
		"||": tokOr,
		"!":  tokNot,
		"!=": tokNe,
		"==": tokEq,
		"<":  tokLt,
		"<=": tokLe,
		">":  tokGt,
		">=": tokGe,
	}

	space  = regexp.MustCompile("^[ \t]+")
	direct = regexp.MustCompile("^#[ \t]*([^ \t]+)")
	ident  = regexp.MustCompile("^[a-zA-Z_]+[a-zA-Z0-9_]*")
	str    = regexp.MustCompile("^(\"[^\"]*\")|(<[^>]*>)")
	num    = regexp.MustCompile("^[0-9]+")
	oper   = regexp.MustCompile("^([()!<>]|&&|[|][|]|!=|==|>=|<=)")
)

type token struct {
	t ttype
	s string
}

func scan(line []byte) []token {
	toks := []token{}
	define := false
	for len(line) > 0 {
		switch {
		case space.Match(line):
			rng := space.FindIndex(line)
			line = line[rng[1]:len(line)]
		case direct.Match(line):
			matches := direct.FindSubmatch(line)
			d := matches[1]
			s := string(d)
			kw := directives[s]
			toks = append(toks, token{kw, s})
			if kw == tokDef {
				define = true
			}
			line = line[len(matches[0]):len(line)]
		case num.Match(line):
			n := num.Find(line)
			toks = append(toks, token{tokNum, string(n)})
			line = line[len(n):len(line)]
		case ident.Match(line):
			id := ident.Find(line)
			s := string(id)
			t, ok := keywords[s]
			if ok {
				toks = append(toks, token{t, s})
			} else {
				toks = append(toks, token{tokId, s})
			}
			line = line[len(id):len(line)]
			if define && len(chompspace(line)) > 0  {
				// just grab the rest of the line for a define
				toks = append(toks, token{tokId, string(chompspace(line))})
				line = []byte{}
			}
		// There is an issue between <, <= and <blah.h>
		case toks[len(toks)-1].t != tokInc && oper.Match(line):
			op := oper.Find(line)
			s := string(op)
			optok := opers[s]
			if define && optok == tokOparen {
				// macro!
				toks = append(toks, token{tokId, string(chompspace(line))})
				line = []byte{}
			} else {
				toks = append(toks, token{optok, s})
				line = line[len(op):len(line)]
			}
		case str.Match(line):
			st := str.Find(line)
			s := string(st)
			toks = append(toks, token{tokStr, s})
			line = line[len(st):len(line)]
		default:
			panic(fmt.Sprintf("line %d: malformed %s", lineno, line))
		}
	}
	return toks
}

type ndtype ttype

type nd struct {
	t           ndtype
	sval        string
	nval        int
	left, right *nd
}

func (t ndtype) String() string {
	return tokstr[ttype(t)]
}

func (n *nd) String() string {
	var s string
	switch n.t {
	case tokIf, tokNot:
		s = fmt.Sprintf("left: %s", n.left.String())
	case tokDef:
		if n.right != nil {
			s = fmt.Sprintf("sval: %s, left: %s",
				n.sval, n.left.String())
		} else {
			s = fmt.Sprintf("sval: %s", n.sval)
		}
	case tokEq, tokNe, tokGe, tokLe, tokLt, tokGt, tokAnd, tokOr:
		s = fmt.Sprintf("left: %s, right: %s", n.left.String(), n.right.String())
	case tokNum, tokIfdef, tokIfndef, tokUndef:
		s = fmt.Sprintf("sval: %s, nval: %d", n.sval, n.nval)
	case tokStr, tokId, tokDefed:
		s = fmt.Sprintf("sval: %s", n.sval)
	}
	return fmt.Sprintf("{t: %s, %s}", n.t.String(), s)
}

func parse(toks []token) *nd {
	switch toks[0].t {
	case tokInc:
		return parseInc(toks)
	case tokIfdef:
		return parseIfdef(toks)
	case tokIfndef:
		return parseIfndef(toks)
	case tokDef:
		return parseDef(toks)
	case tokUndef:
		return parseUndef(toks)
	case tokIf:
		return parseIf(toks)
	case tokElse:
		_, toks := expect(toks, tokElse)
		eol(toks)
		return &nd{t: tokElse}
	case tokEndif:
		_, toks := expect(toks, tokEndif)
		eol(toks)
		return &nd{t: tokEndif}
	default:
		panic(fmt.Sprintf("line %d: malformed directive", lineno))
	}

	panic(fmt.Sprintf("Invalid look-ahead token %v", toks[0].t))
}

func parseInc(toks []token) *nd {
	_, toks = expect(toks, tokInc)
	vl, toks := parseVal(toks)
	eol(toks)
	return &nd{t: tokInc, left: vl}
}

func parseIfdef(toks []token) *nd {
	_, toks = expect(toks, tokIfdef)
	id, toks := expect(toks, tokId)
	eol(toks)
	return &nd{t: tokIfdef, sval: id.s}
}

func parseIfndef(toks []token) *nd {
	_, toks = expect(toks, tokIfndef)
	id, toks := expect(toks, tokId)
	eol(toks)
	return &nd{t: tokIfndef, sval: id.s}
}

func parseDef(toks []token) *nd {
	_, toks = expect(toks, tokDef)
	id, toks := expect(toks, tokId)
	if len(toks) == 1 {
		vl, toks := parseVal(toks)
		eol(toks)
		return &nd{t: tokDef, sval: id.s, left: vl}
	}
	eol(toks)
	return &nd{t: tokDef, sval: id.s}
}

func parseUndef(toks []token) *nd {
	_, toks = expect(toks, tokUndef)
	id, toks := expect(toks, tokId)
	eol(toks)
	return &nd{t: tokUndef, sval: id.s}
}

func parseIf(toks []token) *nd {
	_, toks = expect(toks, tokIf)
	exp, toks := parseExpr(toks)
	eol(toks)
	return &nd{t: tokIf, left: exp}
}

func parseDefed(toks []token) (*nd, []token) {
	_, toks = expect(toks, tokDefed)
	var id token
	if toks[0].t == tokOparen {
		_, toks = expect(toks, tokOparen)
		id, toks = expect(toks, tokId)
		_, toks = expect(toks, tokCparen)
	} else {
		id, toks = expect(toks, tokId)
	}
	return &nd{t: tokDefed, sval: id.s}, toks
}

func parseVal(toks []token) (n *nd, rest []token) {
	t := toks[0]
	rest = toks[1:len(toks)]
	switch t.t {
	case tokStr:
		n = &nd{t: tokStr, sval: t.s}
	case tokNum:
		s := toks[0].s
		i, err := strconv.Atoi(s)
		if err != nil {
			panic(fmt.Sprintf("line %d: can't convert %s to an int", lineno, s))
		}
		n = &nd{t: tokNum, sval: s, nval: i}
	case tokId:
		n = &nd{t: tokId, sval: t.s}
	default:
		panic(fmt.Sprintf("line %d: expected value", lineno))
	}
	return
}

func parseExpr(toks []token) (*nd, []token) {
	conj, toks := parseConj(toks)
	return parseDisj_aux(conj, toks)
}

func parseDisj_aux(l *nd, toks []token) (*nd, []token) {
	if len(toks) > 0 && toks[0].t == tokOr {
		_, toks = expect(toks, tokOr)
		r, toks := parseConj(toks)
		nd := &nd{t: tokOr, left: l, right: r}
		return parseDisj_aux(nd, toks)
	}
	return l, toks
}

func parseConj(toks []token) (*nd, []token) {
	f, toks := parseExpr_aux(toks)
	return parseConj_aux(f, toks)
}

func parseConj_aux(l *nd, toks []token) (*nd, []token) {
	if len(toks) > 0 && toks[0].t == tokAnd {
		_, toks = expect(toks, tokAnd)
		r, toks := parseExpr_aux(toks)
		nd := &nd{t: tokAnd, left: l, right: r}
		return parseConj_aux(nd, toks)
	}
	return l, toks
}

// ( expr ) or <number> or <id> or <defined> and also !
func parseExpr_aux(toks []token) (*nd, []token) {
	switch toks[0].t {
	case tokOparen:
		_, toks = expect(toks, tokOparen)
		n, toks := parseExpr(toks)
		_, toks = expect(toks, tokCparen)
		return n, toks
	case tokNum, tokId, tokDefed:
		return parseRel(toks)
	case tokNot:
		_, toks = expect(toks, tokNot)
		n, toks := parseExpr_aux(toks)
		return &nd{t: tokNot, left: n}, toks
	}
	panic(fmt.Sprintf("line %d: invalid expression: %v", lineno, toks))
}

var binops = map[ttype]ndtype{
	tokEq: tokEq,
	tokNe: tokNe,
	tokGe: tokGe,
	tokLe: tokLe,
	tokLt: tokLt,
	tokGt: tokGt,
}

func parseRel(toks []token) (*nd, []token) {
	switch toks[0].t {
	case tokNot:
		t, toks := parseRel(toks[1:len(toks)])
		return &nd{t: tokNot, left: t}, toks
	case tokDefed:
		return parseDefed(toks)
	case tokId:
		lid, toks := expect(toks, tokId)
		l := &nd{t: tokId, sval: lid.s}
		if len(toks) == 0 {
			return l, toks
		}
		op, ok := binops[toks[0].t]
		if !ok {
			return l, toks
		}
		toks = toks[1:len(toks)]
		r, toks := parseVal(toks)
		return &nd{t: op, left: l, right: r}, toks
	case tokNum:
		l, toks := parseVal(toks)
		if len(toks) == 0 {
			return l, toks
		}
		op, ok := binops[toks[0].t]
		if !ok {
			return l, toks
		}
		toks = toks[1:len(toks)]
		r, toks := parseVal(toks)
		return &nd{t: op, left: l, right: r}, toks
	}

	panic(fmt.Sprintf("line %d: invalid term", lineno))
}

func eol(toks []token) {
	if len(toks) > 0 {
		panic(fmt.Sprintf("line %d: extra tokens at end of line: %v",
			lineno, toks))
	}
}

func expect(toks []token, t ttype) (token, []token) {
	if len(toks) == 0 {
		panic(fmt.Sprintf("line %d: expected %s, got end of file",
			lineno, tokstr[toks[0].t]))
	}
	if toks[0].t != t {
		panic(fmt.Sprintf("line %d: expected %s, got %s", lineno,
			tokstr[t], tokstr[toks[0].t]))
	}
	return toks[0], toks[1:len(toks)]
}

