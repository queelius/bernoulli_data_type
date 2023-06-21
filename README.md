For more information, see:
This is a repo that I will use to develop my Bernoulli data type concept, which is a general framework for understanding and constructing
a lot of interesting new data types and compuational trade-offs. It can also be used as a foundation for constructing oblivious programs
as a composition of oblivious data types / obvious functions.

I doubt any of it will compile -- I actually hadn't looked at this in years, while I battled sickness and cancer, but now that I'm feeling
better, I'm revisting some old projects. Look at my repo for other things I'm working on, and https://metafunctor.com for my personal web site.

Here are some markdown files in the repo. I've throw it all into Doxygen, but I haven't had time to make it work properly yet.

- [Bernoulli Map](BERNOULLI_MAP.md) for info on the most generic type, which can be used to in theory model any computable function.
We also show how even traditional algorithms, like the Miller-Rabin primality test, can be understood in th framework of the Bernoulli model.
- [Bernoulli Bool](BERNOULLI_BOOL.md) for an example of the simplest Bernoulli type, useful for understanding.
- [Bernoulli Set](BERNOULLI_SET.md) for information on the most common kind of Bernoulli data type. The Bloom filtr is a special case.
- [Codec](CODEC.md) for more information codecs. I will eventually use this to describe how to automatically generate any Bernoulli map,
including Cipher maps (Oblivious data type). The idea will be to use the universal Bernoulli map constructor
- [Regular Type](REGULAR_TYPE_CONCEPT.md) for more information about regular types, and how the Bernoulli model fails this basic requirement
in many ways. It's not intended as a criticism, because in some ways it's desirable (say in the oblivious type use-case), only to
explain why some predicates like equality are not necessarily accurate (equality itself return a Bernoulli Boolean).
