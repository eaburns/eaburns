package main

import (
	"bytes"
	"code.google.com/p/eaburns/irc"
	"code.google.com/p/goplan9/plan9/acme"
	"log"
	"strings"
	"time"
	"unicode/utf8"
)

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
	key := strings.ToLower(target)
	w, ok := wins[key]
	if !ok {
		w = newWindow(target)
		wins[key] = w
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

	if who != *nick && strings.Contains(text, *nick) {
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

// Deleting moves the addresses around when
// text is deleted from the window.
func (w *win) deleting(q0, q1 int) {
	if q0 >= w.eAddr { // Deleting entirely after the entry point.
		return
	}
	if q1 >= w.eAddr {
		w.eAddr = q0
	} else {
		w.eAddr -= q1 - q0
	}
	if q0 < w.pAddr {
		if q1 >= w.pAddr {
			w.pAddr = q0
		} else {
			w.pAddr -= q1 - q0
		}
	}
	if q1 < w.pAddr {	// Deleting entirely before the prompt
		return
	}

	w.Addr("#%d,#%d", w.pAddr, w.eAddr)
	w.writeData([]byte(prompt))
	w.eAddr = w.pAddr + utf8.RuneCountInString(prompt)
}

// Del deletes this window.
func (w *win) del() {
	delete(wins, w.target)
	w.Ctl("delete")
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
