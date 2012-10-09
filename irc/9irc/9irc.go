package main

import (
	"code.google.com/p/eaburns/irc"
	"code.google.com/p/goplan9/plan9/acme"
	"flag"
	"fmt"
	"log"
	"os"
	"strings"
	"sort"
	"time"
)

var (
	nick = flag.String("n", os.Getenv("USER"), "nick name")
	full = flag.String("f", os.Getenv("USER"), "full name")
	pass = flag.String("p", "", "password")
	debug = flag.Bool("d", false, "debugging")
)

var (
	// client is the IRC client connection.
	client *irc.Client

	// serverWin is the server window.
	serverWin *window

	// windows contains the windows, indexed by their targets.
	windows = map[string]*window{}

	// winEvents multiplexes all window events.
	winEvents = make(chan windowEvent)

	// users contains all known users.
	users = map[string]*user{}
)

func main() {
	flag.Parse()
	if len(flag.Args()) != 1 {
		os.Stdout.WriteString("usage: 9irc [-f fullname] [-n nick] <server>\n")
		os.Exit(1)
	}

	serverWin = newWindow("")

	var err error
	client, err = irc.DialServer(flag.Arg(0)+":6667", *nick, *full, *pass)
	if err != nil {
		log.Fatal(err)
	}
	serverWin.Fprintf("tag", "Join ")

	for {
		select {
		case ev := <- winEvents:
			handleWindowEvent(ev)

		case msg, ok := <-client.In:		
			if !ok {
				serverWin.WriteString("Disconnected")
				os.Exit(0)
			}
			handleMsg(msg)
		}
	}
}

// window is an open acme window.
type window struct {
	*acme.Win

	// target is the target of this window.  It
	// is either a channel name, a nick name
	// or empty (for the server window).
	target string

	// users is all users currently in this channel
	// excluding one's self.
	users map[string]*user

	// who is a list of users gathered by a who
	// command.
	who []string

	// lastMsgOrigin is the nick name of the last
	// user to send a private message.
	lastMsgOrigin string

	// lastMsgTime is the time at which the last
	// private message was sent.
	lastMsgTime time.Time
}

// windowEvent is an event coming in on a window.
type windowEvent struct {
	*window
	*acme.Event
}

// getWindow returns the window for the given target.
// If the window already exists then it is returned,
// otherwise it is created.
func getWindow(target string) *window {
	w, ok := windows[target]
	if !ok {
		w = newWindow(target)
		windows[target] = w
	}
	return w
}

// newWindow creates a new acme window.
func newWindow(target string) *window {
	win, err := acme.New()
	if err != nil {
		log.Fatal(err)
	}
	name := "/irc/" + flag.Arg(0)
	if target != "" {
		name += "/" + target
	}
	win.Name(name)
	win.Ctl("clean")
	if len(target) > 0 && target[0] == '#' {
		win.Fprintf("tag", "Who ")
	}

	w := &window {
		Win: win,
		target: target,
		users: make(map[string]*user),
		lastMsgTime: time.Now(),
	}
	go func() {	
		for ev := range win.EventChan() {
			winEvents <- windowEvent{w, ev}
		}
	}()

	return w
}

// WriteString writes a string to the body of a window.
func (w *window) WriteString(str string) error {
	const maxWrite = 512
	bytes := []byte(str + "\n")
	for len(bytes) > 0 {
		sz := len(bytes)
		if sz > maxWrite {
			sz = maxWrite
		}
		n, err := w.Write("body", bytes[:sz])
		if err != nil {
			return err
		}
		bytes = bytes[n:]
	}
	return nil
}

// user has information on a single user.
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

// getUser returns the nick for the given user.
// If there is no known user for this nick then
// one is created.
func getUser(nick string) *user {
	u, ok := users[nick]
	if !ok {
		u = &user{nick: nick, origNick: nick, lastChange: time.Now() }
		users[nick] = u
	}
	return u
}

// handleWindowEvent handles events from
// any of the acme windows.
func handleWindowEvent(ev windowEvent) {
	if *debug {
		log.Printf("%#v\n\n", *ev.Event)
	}
	if ev.C2 == 'x' || ev.C2 == 'X' {	// execute tag or body
		fs := strings.Fields(string(ev.Text))
		if len(fs) > 0 {
			handleExecute(ev, fs[0], fs[1:])
		}
	}
}

// handleExecute handles acme execte commands.
func handleExecute(ev windowEvent, cmd string, args []string) {
	switch cmd {
	case "Del":
		t := ev.target
		if ev.window == serverWin {
			client.Out <- irc.Msg{ Cmd: irc.QUIT }
			serverWin.Ctl("delete")
		} else if t != "" && t[0] == '#' {	// channel
			client.Out <- irc.Msg{ Cmd: irc.PART, Args: []string{t} }
		} else {	// private message
			delete(windows, ev.window.target)
			ev.window.Ctl("delete")
		}

	case "Join":
		if len(args) != 1{
			break
		}
		client.Out <- irc.Msg{ Cmd: irc.JOIN, Args: []string{args[0]} }

	case "Nick":
		if len(args) != 1 {
			break
		}
		client.Out <- irc.Msg{ Cmd: irc.NICK, Args: []string{args[0]} }

	case "Msg":
		str := string(ev.Text)[len("Msg"):]
		str = strings.TrimLeft(str, " \t")
		if msg, err := irc.ParseMsg(str); err != nil {
			log.Println(err.Error())
		} else {
			client.Out <- msg
		}

	case "Who":
		if ev.target[0] != '#' {
			break
		}
		ev.window.who = []string{}
		client.Out <- irc.Msg{ Cmd: irc.WHO, Args: []string{ev.target} }
	}
}

// handleMsg handles IRC messages from
// the server.
func handleMsg(msg irc.Msg) {
	if *debug {
		log.Printf("%#v\n\n", msg)
	}
	switch msg.Cmd {
	case irc.ERROR:
		os.Exit(0)

	case irc.PING:
		client.Out <- irc.Msg{ Cmd: irc.PONG }

	case irc.RPL_MOTD:
		serverWin.WriteString(lastArg(msg))

	case irc.RPL_NAMREPLY:
		doRplNamReply(msg.Args[len(msg.Args)-2], lastArg(msg))		

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

func doRplNamReply(ch string, names string) {
	for _, n := range strings.Fields(names) {
		n = strings.TrimLeft(n, "@+")
		if n != *nick {
			doJoin(ch, n)
		}
	}
}

func doJoin(ch, who string) {
	w := getWindow(ch)
	w.WriteString("+" + who)
	if who != *nick {
		u := getUser(who)
		w.users[who] = u
		u.nChans++
	}
}

func doPart(ch, who string) {
	w, ok := windows[ch]
	if !ok {
		return
	}
	if who == *nick {
		w.Ctl("delete")
		delete(windows, w.target)
	} else {
		w.WriteString("-" + who)
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
	for _, w := range windows {
		if _, ok := w.users[who]; !ok {
			continue
		}
		delete(w.users, who)
		s := "-" + who + " quit"
		if txt != "" {
			s += ": " + txt
		}
		w.WriteString(s)
	}
}

func doPrivMsg(ch, who, text string) {
	if ch == *nick {
		ch = who
	}
	w := getWindow(ch)

	const actionPrefix = "\x01ACTION"
	if strings.HasPrefix(text, actionPrefix) {
		text = strings.TrimRight(text[len(actionPrefix):], "\x01")
		w.WriteString(who + text)
		w.lastMsgOrigin = ""
		w.lastMsgTime = time.Now()
		return
	}

	// Only print the user name if there is a
	// new speaker or if two minutes has passed.
	if who != w.lastMsgOrigin || time.Since(w.lastMsgTime).Minutes() > 2 {
		origNick := ""
		if u, ok := users[who]; ok {
			// If the user hasn't change their nick in an hour
			// then set this as the original nick name.
			if time.Since(u.lastChange).Hours() > 1 {
				u.origNick = u.nick
			}
			if u.nick != u.origNick {
				origNick = " (" + u.origNick + ")"
			}
		}
		w.WriteString(fmt.Sprintf("<%s>%s", who, origNick))
	}
	w.lastMsgOrigin = who
	w.lastMsgTime = time.Now()

	if strings.HasPrefix(text, *nick + ":") {
		w.WriteString("*\t" + text)
	} else {
		w.WriteString("\t" + text)
	}
}

func doNick(prev, cur string) {
	if prev == *nick {
		*nick = cur
		return
	}

	u := users[prev]
	delete(users, prev)
	users[cur] = u
	u.nick = cur
	u.lastChange = time.Now()

	for _, w := range windows {
		if _, ok := w.users[prev]; !ok {
			continue
		}
		delete(w.users, prev)
		w.users[cur] = u
		w.WriteString(prev + " -> " + cur)
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
	s := ""
	sort.Strings(w.who)
	for i, n := range w.who {
		if i % 4 == 0 {
			if i > 0 {
				s += "\n"
			}
		} else {
			s += " "
		}
		s += "[" + n + "]"
	}
	w.who = []string{}
	w.WriteString(s)
}

// lastArg returns the last message
// argument or the empty string if there
// are no arguments.
func lastArg(msg irc.Msg) string {
	if len(msg.Args) == 0 {
		return ""
	}
	return msg.Args[len(msg.Args)-1]
}