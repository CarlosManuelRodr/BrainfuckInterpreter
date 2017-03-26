# Brainfuck Interpreter
Simple and unoptimized brainfuck interpreter with focus on creating custom interfaces and encoding text.

## About the interpreter
The only thing special about this particular implementation is that you can set a particular language interface, so instead of a Hello World code that looks like

```
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```
you can map every instruction to a word so you can have

```
A a a a a a a a -ish this a a a a -ish this a a this a a a this a a a this a is is is is simple language this a this a this simple this this a -ish is language is simple language this this custom this simple simple simple custom a a a a a a a custom custom a a a custom this this custom is simple custom is custom a a a custom simple simple simple simple simple simple custom simple simple simple simple simple simple simple simple custom this this a custom this a a custom.
```