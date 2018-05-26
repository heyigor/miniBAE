### Twang!
Today, with permission from Beatnik, Inc, I am releasing a code base, loved and hated by millions. It has probably been in a device in your pocket. We call it miniBAE, short for mini Beatnik Audio Engine.

First, let me apologize. Set your wayback machine to the year 2000. Nokia, the king of mobile phones, and their default Ringtone. [en.wikipedia.org/wiki/Nokia_tune](http://en.wikipedia.org/wiki/Nokia_tune)

The Ringtone heard around the world. My fault. Not that its bad, but after a billion phones and countless trillion plays, maximum exposure has a new limit. The code being released today was responsible for enabling that audio meme to a wide audience.

As with any technology, there’s a story, and this one was an interesting one. The first version was created in 1991 by my business partner, Jim Nitchals, and myself. At that point it was called SoundMusicSys. Appearing in many successful video games for the Macintosh OS. Prince of Persia, Lemmings, Sim City to name a few. In 1997 we joined forces with Beatnik, Inc, founded by Thomas Dolby, and ended up in iconic consumer electronics from Apple, Sun’s Java, MagicCap, WebTV, BeOS, Nokia, Motorola, Danger and others.

We both were obsessed with creating something that allows music to be played that was small in size. We ended up creating the first software wave table synthesizer that read and played standard Midi files all done in software. Prior to our technology, the only solution was hardware. A dedicate sound card.

But because of our background in video games, we understood low latency interactive audio and music, and the end result is this code. Before OpenAL, PortAudio, before many audio hardware abstraction layers there was miniBAE.

The last commercial product that this code was used in was the Danger, Inc, T-Mobile Sidekick 3 in 2005.

The motives in releasing the code into the open source community are many:

1. Jim and I believed in the Jeffersonian ideal that a inventor has a lock on an idea for a limited period of time. There were many ideas in this code that could have been patented, however we chose not to pursue it. We chose the Trade Secret route. Because of that, this code did not end up in the hands of a Patent troll. Now it can be mixed back into the culture, to be recycled and reinvented.

2. This engine is perfect for anything that requires small storage to create audio. Midi is a standard and is tiny. It does require great skill and effort to craft something beautiful, but it can be done. Along with this package are countless examples of Craftsmen and Musicians doing their best to create 5 minutes of music in 100k bytes.

3. There were some outstanding people involved in this technology from 1991 to 2005, and I want to acknowledge them. Without their passion, belief, and considerable help, this code would have never gotten far.

4. Because now its available, some of the interesting DSP algorithms can be reused. File formats, fast reverbs, fast chorus, fast sample rate converters (SRC).

5. I want to use it!

6. Both Jim and I are digital historians, and this code has some small value in preserving. And the best way to do that is release it as a BSD license.

7. I have a small hope that our mission at Beatnik, to sonify the web, can become a reality now that the entire software stack is available. From the Javascript musicObject API, down to the C code that does the mixing.

Enjoy.
