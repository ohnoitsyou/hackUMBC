hackUMBC
========
Inspiration from: http://www.instructables.com/id/Minimalistic-LED-wall-light/

This is the code I originaly tried to use for my two arduinos.
The intent was to connect them using a pair of NRF24L01's to have them talk back and forth.
Something is really broken in the code. I welcome any pull requests that can help me fix it!

If the units fail to communicate, they continue to work fine. no data is sent, but the application still runs.
If they successfully communicate twice, at least the master node will stop working. Just fails to continue to loop.
I had a couple people look at it while I was at hackUMBC and no one could explain it.

I didn't have time to continue to debug it, and switched to using a Spark Core, which I absolutely LOVE!

I'm going to put up my code for the spark and for the server in the next couple days. I'll link to it once it's up.

You can find my (relatively) short blog post about my struggles on [my blog](blog.ohnoitsyou.net/hackumbc)
