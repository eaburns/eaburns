// 9irc is an IRC client for the acme editor.
package main

import (
	"bytes"
	"code.google.com/p/eaburns/irc"
	"code.google.com/p/goplan9/plan9/acme"
	"flag"
	"log"
	"net"
	"io"
	"os"
	"sort"
	"strings"
	"time"
	"unicode/utf8"
)

const defaultPort = "6667"

const prompt = "\n>"

const meCmd = "/me"

var (
	nick   = flag.String("n", os.Getenv("USER"), "nick name")
	full   = flag.String("f", os.Getenv("USER"), "full name")
	pass   = flag.String("p", "", "password")
	debug  = flag.Bool("d", false, "debugging")
	server = ""
)

var (
	// client is the IRC client connection.
	client *irc.Client

	// serverWin is the server win.
	serverWin *win

	// wins contains the wins, indexed by their targets.
	wins = map[string]*win{}

	// winEvents multiplexes all win events.
	winEvents = make(chan winEvent)

	// users contains all known users.
	users = map[string]*user{}
)

func main() {
	flag.Usage = func() {
		os.Stdout.WriteString("usage: 9irc [options] <server>[:<port>]\n")
		flag.PrintDefaults()
	}
	flag.Parse()
	if len(flag.Args()) != 1 {
		flag.Usage()
		os.Exit(1)
	}

	var err error
	var port string
	if server, port, err = net.SplitHostPort(flag.Arg(0)); err != nil {
		port = defaultPort
		server = flag.Arg(0)
	}

	serverWin = newWindow("")
	client, err = irc.DialServer(server+":"+port, *nick, *full, *pass)
	if err != nil {
		log.Fatal(err)
	}
	serverWin.Fprintf("tag", "Chat ")

	// Set Dump handling for the server window.
	if wd, err := os.Getwd(); err != nil {
		log.Println(err)
	} else {
		serverWin.Ctl("dumpdir %s", wd)
	}
	serverWin.Ctl("dump %s", strings.Join(os.Args, " "))

out:
	for {
		select {
		case ev := <-winEvents:
			handleWindowEvent(ev)

		case msg, ok := <-client.In:
			if !ok {
				break out
			}
			handleMsg(msg)

		case err, ok := <-client.Errors:
			if ok && err != io.EOF {
				log.Println(err)
			}
		}
	}
	for _, w := range wins {
		w.WriteString("Disconnected")
		w.Ctl("clean")
	}
	serverWin.WriteString("Disconnected")
	serverWin.Ctl("clean")
}

// Win is an open acme windown for either
// the server, a channel, or a private message.
type win struct {
	*acme.Win

	// PromptAddr is the address of the empty
	// string just before the prompt.  This is
	// the address at which incoming messages
	// will be displayed.
	pAddr int

	// EntryAddr is the address of the empty
	// byte just after the prompt after which
	// is the user's input.
	eAddr int

	// Target is the target of this win.  It
	// is either a channel name, a nick name
	// or empty (for the server win).
	target string

	// Users is all users currently in this channel
	// excluding one's self.
	users map[string]*user

	// Who is a list of users gathered by a who
	// command.
	who []string

	// LastMsgOrigin is the nick name of the last
	// user to send a private message.
	lastSpeaker string

	// LastMsgTime is the time at which the last
	// private message was sent.
	lastTime time.Time
}

// WinEvent is an event coming in on a win.
type winEvent struct {
	*win
	*acme.Event
}

// GetWindow returns the win for the given target.
// If the win already exists then it is returned,
// otherwise it is created.
func getWindow(target string) *win {
	w, ok := wins[target]
	if !ok {
		w = newWindow(target)
		wins[target] = w
	}
	return w
}

// NewWindow creates a new win and starts
// a go routine sending its events to the
// winEvents channel.
func newWindow(target string) *win {
	aw, err := acme.New()
	if err != nil {
		log.Fatal(err)
	}
	name := "/irc/" + server
	if target != "" {
		name += "/" + target
	}
	aw.Name(name)
	aw.Ctl("clean")
	aw.Write("body", []byte(prompt))
	if len(target) > 0 && target[0] == '#' {
		aw.Fprintf("tag", "Who ")
	}

	w := &win{
		Win:      aw,
		eAddr:    utf8.RuneCountInString(prompt),
		target:   target,
		users:    make(map[string]*user),
		lastTime: time.Now(),
	}
	go func() {
		for ev := range aw.EventChan() {
			winEvents <- winEvent{w, ev}
		}
	}()
	return w
}

const actionPrefix = "\x01ACTION"

// PrivMsgString returns the string that should
// be written to this win for a message.
func (w *win) privMsgString(who, text string) string {
	if text == "\n" {
		return ""
	}

	if strings.HasPrefix(text, actionPrefix) {
		text = strings.TrimRight(text[len(actionPrefix):], "\x01")
		if w.lastSpeaker != who {
			w.lastSpeaker = ""
		}
		w.lastTime = time.Now()
		return "*" + who + text
	}

	buf := bytes.NewBuffer(make([]byte, 0, 512))

	// Only print the user name if there is a
	// new speaker or if two minutes has passed.
	if who != w.lastSpeaker || time.Since(w.lastTime).Minutes() > 2 {
		buf.WriteRune('<')
		buf.WriteString(who)
		buf.WriteRune('>')

		if u, ok := users[who]; ok {
			// If the user hasn't change their nick in an hour
			// then set this as the original nick name.
			if time.Since(u.lastChange).Hours() > 1 {
				u.origNick = u.nick
			}
			if u.nick != u.origNick {
				buf.WriteString(" (")
				buf.WriteString(u.origNick)
				buf.WriteRune(')')
			}
		}
		buf.WriteRune('\n')
	}
	w.lastSpeaker = who
	w.lastTime = time.Now()

	if strings.HasPrefix(text, *nick+":") {
		buf.WriteRune('!')
	}
	buf.WriteRune('\t')
	buf.WriteString(text)
	return buf.String()
}

// WritePrivMsg writes the private message text
// to the window.  The message is decorated with
// the name of the sender unless the last message
// to this window was from the same sender within
// a specified time.
func (w *win) writePrivMsg(who, text string) {
	w.WriteString(w.privMsgString(who, text))
}

// WriteMsg writes non-private message text to
// the window.
func (w *win) writeMsg(text string) {
	w.WriteString(text)
	w.lastSpeaker = ""
}

// WriteString writes a string to the body of a win.
func (w *win) WriteString(str string) {
	w.Addr("#%d", w.pAddr)
	data := []byte(str + "\n")
	w.writeData(data)

	nr := utf8.RuneCount(data)
	w.pAddr += nr
	w.eAddr += nr
}

// WriteData writes all of the given bytes to the
// data file.  Uses a chunk size that is small enough
// that acme won't choke on it.
func (w *win) writeData(data []byte) {
	const maxWrite = 512
	for len(data) > 0 {
		sz := len(data)
		if sz > maxWrite {
			sz = maxWrite
		}
		n, err := w.Write("data", data[:sz])
		if err != nil {
			log.Fatal(err)
		}
		data = data[n:]
	}
}

// Typing moves addresses around when text
// is typed.  If the the user enters a newline after
// the prompt then the text is sent to the
// target of the window.
func (w *win) typing(q0, q1 int) {
	if q0 < w.pAddr {
		w.pAddr += q1 - q0
		// w.textAddr >= w.pAddr so this
		// call returns in the next if clause.
	}
	if q0 < w.eAddr {
		w.eAddr += q1 - q0
		return
	}

	w.Addr("#%d", w.eAddr)
	text, err := w.ReadAll("data")
	if err != nil {
		log.Fatal(err)
	}
	for {
		i := bytes.IndexRune(text, '\n')
		if i < 0 {
			break
		}

		t := string(text[:i+1])
		w.Addr("#%d,#%d", w.pAddr, w.eAddr+utf8.RuneCountInString(t))
		if strings.HasPrefix(t, meCmd) {
			act := strings.TrimLeft(t[len(meCmd):], " \t")
			act = strings.TrimRight(act, "\n")
			if act == "\n" {
				t = "\n"
			} else {
				t = actionPrefix + " " + act + "\x01"
			}
		}

		msg := ""
		if w == serverWin {
			if msg = t; msg == "\n" {
				msg = ""
			}
		} else {
			msg = w.privMsgString(*nick, t)

			// Always tack on a newline.
			// In the case of a /me command, the
			// newline will be missing, it is added
			// here.
			if len(msg) > 0 && msg[len(msg)-1] != '\n' {
				msg = msg + "\n"
			}
		}
		w.writeData([]byte(msg + prompt))

		w.pAddr += utf8.RuneCountInString(msg)
		w.eAddr = w.pAddr + utf8.RuneCountInString(prompt)
		text = text[i+1:]

		if t == "\n" {
			continue
		}
		if w == serverWin {
			sendRawMsg(t)
		} else {
			// BUG(eaburns): Long PRIVMSGs should be broken up and sent in pieces.
			client.Out <- irc.Msg{
				Cmd:  "PRIVMSG",
				Args: []string{w.target, t},
			}
		}
	}
	w.Addr("#%d", w.pAddr)
}

// Deleting moves the addresses around when
// text is deleted from the window.
func (w *win) deleting(q0, q1 int) {
	if q0 >= w.eAddr {
		return
	}
	if q1 >= w.eAddr {
		w.eAddr = q0
	} else {
		w.eAddr -= q1 - q0
	}

	if q0 >= w.pAddr {
		return
	}
	if q1 >= w.pAddr {
		w.pAddr = q0
	} else {
		w.pAddr -= q1 - q0
	}
}

// User has information on a single user.
type user struct {
	// nick is the user's current nick name.
	nick string

	// origNick is the user's original nick name.
	origNick string

	// lastChange is the time at which the user
	// last changed their name.  After some
	// amount of time set the current nick to
	// the user's origNick.
	lastChange time.Time

	// nChans is the number of channels in
	// common with this user.
	nChans int
}

// GetUser returns the nick for the given user.
// If there is no known user for this nick then
// one is created.
func getUser(nick string) *user {
	u, ok := users[nick]
	if !ok {
		u = &user{nick: nick, origNick: nick, lastChange: time.Now()}
		users[nick] = u
	}
	return u
}

// HandleWindowEvent handles events from
// any of the acme wins.
func handleWindowEvent(ev winEvent) {
	if *debug {
		log.Printf("%#v\n\n", *ev.Event)
	}

	switch {
	case ev.C2 == 'x' || ev.C2 == 'X':
		fs := strings.Fields(string(ev.Text))
		if len(fs) > 0 && handleExecute(ev, fs[0], fs[1:]) {
			return
		}
		ev.WriteEvent(ev.Event)

	case (ev.C1 == 'M' || ev.C1 == 'K') && ev.C2 == 'I':
		ev.typing(ev.Q0, ev.Q1)

	case (ev.C1 == 'M' || ev.C1 == 'K') && ev.C2 == 'D':
		ev.deleting(ev.Q0, ev.Q1)

	case ev.C2 == 'l' || ev.C2 == 'L':
		if ev.Flag & 2 != 0 {	// expansion
			// The look was on highlighted text.  Instead of
			// sending the hilighted text, send the original
			// addresses, so that 3-clicking without draging
			// on selected text doesn't move the cursor
			// out of the tag.
			ev.Q0 = ev.OrigQ0
			ev.Q1 = ev.OrigQ1
		}
		ev.WriteEvent(ev.Event)
	}
}

// HandleExecute handles acme execte commands.
func handleExecute(ev winEvent, cmd string, args []string) bool {
	switch cmd {
	case "Debug":
		*debug = !*debug

	case "Del":
		t := ev.target
		if ev.win == serverWin {
			client.Out <- irc.Msg{Cmd: irc.QUIT}
		} else if t != "" && t[0] == '#' { // channel
			client.Out <- irc.Msg{Cmd: irc.PART, Args: []string{t}}
		} else { // private message
			delete(wins, ev.win.target)
			ev.win.Ctl("delete")
		}

	case "Chat":
		if len(args) != 1 {
			break
		}
		if args[0][0] == '#' {
			client.Out <- irc.Msg{Cmd: irc.JOIN, Args: []string{args[0]}}
		} else { // private message
			getWindow(args[0])
		}

	case "Nick":
		if len(args) != 1 {
			break
		}
		client.Out <- irc.Msg{Cmd: irc.NICK, Args: []string{args[0]}}

	case "Who":
		if ev.target[0] != '#' {
			break
		}
		ev.win.who = []string{}
		client.Out <- irc.Msg{Cmd: irc.WHO, Args: []string{ev.target}}

	default:
		return false
	}

	return true
}

// SendRawMsg sends a raw message to the server.
// If there is an error parsing a message  from the
// string then it is logged.
func sendRawMsg(str string) {
	str = strings.TrimLeft(str, " \t")
	if msg, err := irc.ParseMsg(str); err != nil {
		log.Println(err.Error())
	} else {
		client.Out <- msg
	}
}

// HandleMsg handles IRC messages from
// the server.
func handleMsg(msg irc.Msg) {
	if *debug {
		log.Printf("%#v\n\n", msg)
	}

	switch msg.Cmd {
	case irc.ERROR:
		log.Fatal(msg)

	case irc.PING:
		client.Out <- irc.Msg{Cmd: irc.PONG}

	case irc.RPL_MOTD:
		serverWin.WriteString(lastArg(msg))

	case irc.RPL_NAMREPLY:
		doNamReply(msg.Args[len(msg.Args)-2], lastArg(msg))

	case irc.RPL_TOPIC:
		doTopic(msg.Args[1], msg.Args[0], lastArg(msg))

	case irc.TOPIC:
		doTopic(msg.Args[0], msg.Origin, lastArg(msg))

	case irc.JOIN:
		doJoin(msg.Args[0], msg.Origin)

	case irc.PART:
		doPart(msg.Args[0], msg.Origin)

	case irc.QUIT:
		doQuit(msg.Origin, lastArg(msg))

	case irc.NOTICE:
		serverWin.WriteString("NOTICE: " + msg.Origin + " " + lastArg(msg))

	case irc.PRIVMSG:
		doPrivMsg(msg.Args[0], msg.Origin, msg.Args[1])

	case irc.NICK:
		doNick(msg.Origin, msg.Args[0])

	case irc.RPL_WHOREPLY:
		doWhoReply(msg.Args[1], msg.Args[2:])

	case irc.RPL_ENDOFWHO:
		doEndOfWho(msg.Args[1])

	default:
		cmd := irc.CmdNames[msg.Cmd]
		serverWin.WriteString("(" + cmd + ") " + msg.Raw)
	}
}

func doNamReply(ch string, names string) {
	for _, n := range strings.Fields(names) {
		n = strings.TrimLeft(n, "@+")
		if n != *nick {
			doJoin(ch, n)
		}
	}
}

func doTopic(ch, who, what string) {
	w := getWindow(ch)
	w.writeMsg("=" + who + " topic: " + what)
}

func doJoin(ch, who string) {
	w := getWindow(ch)
	w.writeMsg("+" + who)
	if who != *nick {
		u := getUser(who)
		w.users[who] = u
		u.nChans++
	}
}

func doPart(ch, who string) {
	w, ok := wins[ch]
	if !ok {
		return
	}
	if who == *nick {
		w.Ctl("delete")
		delete(wins, w.target)
	} else {
		w.writeMsg("-" + who)
		delete(w.users, who)
		u := getUser(who)
		u.nChans--
		if u.nChans == 0 {
			delete(users, who)
		}
	}
}

func doQuit(who, txt string) {
	delete(users, who)
	for _, w := range wins {
		if _, ok := w.users[who]; !ok {
			continue
		}
		delete(w.users, who)
		s := "-" + who + " quit"
		if txt != "" {
			s += ": " + txt
		}
		w.writeMsg(s)
	}
}

func doPrivMsg(ch, who, text string) {
	if ch == *nick {
		ch = who
	}
	getWindow(ch).writePrivMsg(who, text)
}

func doNick(prev, cur string) {
	if prev == *nick {
		*nick = cur
		for _, w := range wins {
			w.writeMsg("~" + prev + " -> " + cur)
		}
		return
	}

	u := users[prev]
	delete(users, prev)
	users[cur] = u
	u.nick = cur
	u.lastChange = time.Now()

	for _, w := range wins {
		if _, ok := w.users[prev]; !ok {
			continue
		}
		delete(w.users, prev)
		w.users[cur] = u
		w.writeMsg("~" + prev + " -> " + cur)
	}
}

func doWhoReply(ch string, info []string) {
	w := getWindow(ch)
	s := info[3]
	if strings.IndexRune(info[4], '+') >= 0 {
		s = "+" + s
	}
	if strings.IndexRune(info[4], '@') >= 0 {
		s = "@" + s
	}
	w.who = append(w.who, s)
	serverWin.WriteString(ch + " " + s + " " + info[0] + "@" + info[1])
}

func doEndOfWho(ch string) {
	w := getWindow(ch)
	sort.Strings(w.who)
	w.writeMsg("[" + strings.Join(w.who, "] [") + "]")
	w.who = w.who[:0]
}

// LastArg returns the last message
// argument or the empty string if there
// are no arguments.
func lastArg(msg irc.Msg) string {
	if len(msg.Args) == 0 {
		return ""
	}
	return msg.Args[len(msg.Args)-1]
}
