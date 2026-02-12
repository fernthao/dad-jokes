# Dad Jokes App
The application allows clients to interact with a database of jokes on the server. 

Operations include listing all joke titles, getting a random joke, adding a new joke with a title and content to the list, editing and deletion of an existing joke, and getting a random joke printed as part of an ASCII art representing a dad.

## Usage 
### Client
`./proj5 -h host -p port [-c joke_title -m joke_message] [-l] [-r] [-e joke_title -m joke_message] [-d joke_title] [-s]`

- “-h host” must always be given. This provides the hostname/IP address of the server. 
- “-p port” must always be given. This provides the port that the server is listening at. 
- When "-c joke_title -m joke_message" is given, the program creates a new joke with the given title, supplied as the argument to -c. The -c option must be accompanied by the -m option to provide the joke content.
- When "-l" is given the program lists all jokes stored on the server. 
- When "-r" is given the program retrieves a random joke from the server and prints it out on the console. 
- When "-e joke_title -m joke_message" is given, the program updates an existing joke. The joke is identified by its title, supplied as the argument to the -e option. The -e option must be accompanied by the -m option to provide the new joke content. 
- When "-d joke_title" is given, the program deletes the joke with the specified title from the server.
- When "-s" is given the program prints out a random joke in a speech bubble on top of a winky face ASCII art.

The user must give exactly one of the mode options: "-l", "-c", "-r", "-e", "-d" or “-s”. Multiple mode options are not allowed.

The command line options may appear in any order.

### Server

 `./proj5d port-number`
 
The port-number argument is mandatory, it specifies the port that the server will be listening on. Use a port number larger than 1024. 

If met with error “cannot bind to port <port-number>”, try again with the next number until no error is shown. 

## Additional Notes
Feel free to have at some jokes:
```
Why do programmers like dark mode? Cuz light attracts bugs.

Why did the car take a nap? It was tired.

What genre are national anthems? Country.

I wondered why the baseball was getting closer and closer ... then it hit me.

Why are libraries so tall? Because they have many stories.

Did you hear about the guy who's afraid of escalators? He takes steps to avoid them. 

I was going to tell a sodium joke, then I thought, “Na.”

Why are frogs good at baseball? They know how to catch fly balls.

What did the buffalo say to her son on the first day of school? Bison.

What did the beach say when the tide came in? Long time no sea.

Where do penguins go to vote? The North Poll.

I told a bad chemistry joke once. I got no reaction.

Why did the cow go to Hollywood? To be in the moo-vies.

What kind of fish knows how to do an appendectomy? A Sturgeon.

Where did people hang out during medieval times? At knight clubs.

When does Friday come before Thursday? In the dictionary.

Why shouldn’t you trust trees? They seem shady.
```
