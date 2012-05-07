#include "irc.h"

#include "util/regex.h"

#include <stdexcept>

using namespace Network;
using namespace IRC;

static Command::Type convertCommand(const std::string & cmd){
    Command::Type command = Command::Unknown;
    if (cmd == "PASS"){
        command = Command::Pass;
    } else if (cmd == "NICK"){
        command = Command::Nick;
    } else if (cmd == "USER"){
        command = Command::User;
    } else if (cmd == "SERVER"){
        command = Command::Server;
    } else if (cmd == "OPER"){
        command = Command::Oper;
    } else if (cmd == "QUIT"){
        command = Command::Quit;
    } else if (cmd == "SQUIT"){
        command = Command::Squit;
    } else if (cmd == "JOIN"){
        command = Command::Join;
    } else if (cmd == "PART"){
        command = Command::Part;
    } else if (cmd == "MODE"){
        command = Command::Mode;
    } else if (cmd == "TOPIC"){
        command = Command::Topic;
    } else if (cmd == "NAMES"){
        command = Command::Names;
    } else if (cmd == "LIST"){
        command = Command::List;
    } else if (cmd == "INVITE"){
        command = Command::Invite;
    } else if (cmd == "KICK"){
        command = Command::Kick;
    } else if (cmd == "VERSION"){
        command = Command::Version;
    } else if (cmd == "STATS"){
        command = Command::Stats;
    } else if (cmd == "LINKS"){
        command = Command::Links;
    } else if (cmd == "TIME"){
        command = Command::Time;
    } else if (cmd == "CONNECT"){
        command = Command::Connect;
    } else if (cmd == "TRACE"){
        command = Command::Trace;
    } else if (cmd == "ADMIN"){
        command = Command::Admin;
    } else if (cmd == "INFO"){
        command = Command::Info;
    } else if (cmd == "PRIVMSG"){
        command = Command::PrivateMessage;
    } else if (cmd == "NOTICE"){
        command = Command::Notice;
    } else if (cmd == "WHO"){
        command = Command::Who;
    } else if (cmd == "WHOIS"){
        command = Command::Whois;
    } else if (cmd == "WHOWAS"){
        command = Command::Whowas;
    } else if (cmd == "KILL"){
        command = Command::Kill;
    } else if (cmd == "PING"){
        command = Command::Ping;
    } else if (cmd == "PONG"){
        command = Command::Pong;
    } else if (cmd == "ERROR"){
        command = Command::Error;
    }
    return command;
    
}

static std::string convertCommand(const Command::Type & cmd){
    std::string command;
    switch (cmd){
        case Command::Pass:
            command = "PASS";
            break;
        case Command::Nick:
            command = "NICK";
            break;
        case Command::User:
            command = "USER";
            break;
        case Command::Server:
            command = "SERVER";
            break;
        case Command::Oper:
            command = "OPER";
            break;
        case Command::Quit:
            command = "QUIT";
            break;
        case Command::Squit:
            command = "SQUIT";
            break;
        case Command::Join:
            command = "JOIN";
            break;
        case Command::Part:
            command = "PART";
            break;
        case Command::Mode:
            command = "MODE";
            break;
        case Command::Topic:
            command = "TOPIC";
            break;
        case Command::Names:
            command = "NAMES";
            break;
        case Command::List:
            command = "LIST";
            break;
        case Command::Invite:
            command = "INVITE";
            break;
        case Command::Kick:
            command = "KICK";
            break;
        case Command::Version:
            command = "VERSION";
            break;
        case Command::Stats:
            command = "STATS";
            break;
        case Command::Links:
            command = "LINKS";
            break;
        case Command::Time:
            command = "TIME";
            break;
        case Command::Connect:
            command = "CONNECT";
            break;
        case Command::Trace:
            command = "TRACE";
            break;
        case Command::Admin:
            command = "ADMIN";
            break;
        case Command::Info:
            command = "INFO";
            break;
        case Command::PrivateMessage:
            command = "PRIVMSG";
            break;
        case Command::Notice:
            command = "NOTICE";
            break;
        case Command::Who:
            command = "WHO";
            break;
        case Command::Whois:
            command = "WHOIS";
            break;
        case Command::Whowas:
            command = "WHOAS";
            break;
        case Command::Kill:
            command = "KILL";
            break;
        case Command::Ping:
            command = "PING";
            break;
        case Command::Pong:
            command = "PONG";
            break;
        case Command::Error:
            command = "ERROR";
            break;
        case Command::Unknown:
        default:
            break;
    }
    return command;
}

static std::vector<std::string> split(std::string str, char splitter){
    std::vector<std::string> strings;
    size_t next = str.find(splitter);
    while (next != std::string::npos){
        strings.push_back(str.substr(0, next));
        str = str.substr(next+1);
        next = str.find(splitter);
    }
    if (str != ""){
        strings.push_back(str);
    }

    return strings;
}
    
Command::Command(const std::string & message){
    std::vector< std::string > messageSplit = split(message, ' ');
    std::vector< std::string >::iterator current = messageSplit.begin();
    if (Util::matchRegex(*current, "^:.*")){
        // Found owner (":") indicates the user, otherwise it's going to be the command
        current++;
    }
    // Next is the actual command
    type = convertCommand(*current);
    if (type == Unknown){
        Global::debug(0) << "Got unhandled response: " << *current << std::endl;
    }
    current++;
    // Parameters
    bool concactenate = false;
    std::string concactenated;
    for (std::vector< std::string >::iterator i = current; i != messageSplit.end(); ++i){
        const std::string & parameter = *i;
        // If there is a colon in the parameter the rest of split string is the whole parameter rejoin
        if (Util::matchRegex(parameter, "^:.*") && !concactenate){
            concactenate = true;
        }
        if (concactenate){
            concactenated += parameter + " ";
        } else {
            parameters.push_back(parameter);
        }
    }
    if (concactenate){
        parameters.push_back(concactenated);
    }
}

Command::Command(const std::string & owner, const Type & type):
owner(owner),
type(type){
}

Command::Command(const Command & copy):
owner(copy.owner),
type(copy.type),
parameters(copy.parameters){
}

Command::~Command(){
}

const Command & Command::operator=(const Command & copy){
    owner = copy.owner;
    type = copy.type;
    parameters = copy.parameters;
    return *this;
}

std::string Command::getSendable() const {
    std::string sendable;
    // Name
    if (!owner.empty()){
        sendable+=":" + owner + " ";
    }
    // Command
    sendable+=convertCommand(type) + " ";
    // Params
    /*for (std::vector<std::string>::const_iterator i = parameters.begin(); i != parameters.end(); ++i){
        sendable+= *i + " ";
    }*/
    for (unsigned int i = 0; i < parameters.size(); ++i){
        sendable+=parameters[i] + (i < parameters.size()-1 ? " " : "");
    }
    // End
    sendable+="\r\n";
    return sendable;
}


Client::Client(const std::string & hostname, int port):
username("AUTH"),
hostname(hostname),
port(port),
end(false){
}

Client::~Client(){
}

void Client::connect(){
    if (username.empty()){
        throw NetworkException("Set username first.");
    }
    Global::debug(0) << "Connecting to " << hostname << " on port " << port << std::endl;
    socket = Network::connect(hostname, port);
    start();
    setName("paintown-test");
    Command user("AUTH", Command::User);
    user.setParameters(username, "*", "*", ":"+username);
    sendCommand(user);
    //  ^^^^^^^^ Should get a response from this crap!
    joinChannel("#paintown");
    
    Global::debug(0) << "Connected" << std::endl;
}



bool Client::hasCommands() const{
    ::Util::Thread::ScopedLock scope(lock);
    return !commands.empty();
}

Command Client::nextCommand() const {
    ::Util::Thread::ScopedLock scope(lock);
    Command command = commands.front();
    commands.pop();
    return command;
}

void Client::sendCommand(const Command & command){
    Global::debug(0) << "Sending Message: " << command.getSendable() << std::endl;
    Network::sendStr(socket, command.getSendable());
}

void Client::setName(const std::string & name){
    Command command(username, Command::Nick);
    command.setParameters(name);
    sendCommand(command);
    username = name;
}

void Client::joinChannel(const std::string & chan){
    if (!channel.empty()){
        Command command(username, Command::Part);
        command.setParameters(channel);
        sendCommand(command);
    }
    channel = chan;
    Command command(username, Command::Join);
    command.setParameters(channel);
    sendCommand(command);
}

void Client::sendMessage(const std::string & msg){
    Command message(username, Command::PrivateMessage);
    message.setParameters(channel, ":" + msg);
    sendCommand(message);
}

void Client::sendPong(const Command & ping){
    Command pong(username, Command::Pong);
    pong.setParameters(ping.getParameters());
    sendCommand(pong);
}

std::string Client::readMessage(){
    std::string received;
    bool foundReturn = false;
    while (true){
        try {
            char nextCharacter = Network::read8(socket);
            /* NOTE the latest RFC says that either \r or \n is the end of the message
             * http://www.irchelp.org/irchelp/rfc/chapter8.html
             */
            if (nextCharacter == '\r'){
                // Found return
                foundReturn = true;
                continue;
            } else if ((nextCharacter == '\n') && foundReturn){
                // Should be the end of the message assumin \r is before it
                break;
            }
            received += nextCharacter;
        } catch (const Network::MessageEnd & ex){
            // end of message get out
            throw ex;
        }
    }
    //Global::debug(0) << "Read next string: " << received << std::endl;
    return received;
}

void Client::run(){
    while (!end){
        try {
            const std::string & message = readMessage();
            // Check if the message is empty it might be because of (\n)
            if (!message.empty()){
                Command command(message);
                lock.acquire();
                commands.push(command);
                lock.signal();
                lock.release();
            }
        } catch (const Network::MessageEnd & ex){
            end = true;
        }
    }
}
