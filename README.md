getmss
======

Quickly get the MSS for any site you specify without requiring root. Getmss is a utility that quickly tells you what MSS a connection to a given server uses. Works for v4, v6, etc. Found it handy debugging a few issues.

If you're wondering what MSS means, here's the wikipedia page on the subject : [http://en.wikipedia.org/wiki/Maximum_segment_size].

It's a tiny project. The main advantage is that it's simple, should compile pretty much anywere, and doesn't require any special privileges.

{{{
$ ./getmss google.com
Trying protocol AF_INET6 socktype SOCK_STREAM address 2607:f8b0:4010:801::1005 port 80
Found MSS to be 1398
Trying protocol AF_INET6 socktype SOCK_DGRAM address 2607:f8b0:4010:801::1005 port 80
Failed to get MSS: Protocol not available
}}}
