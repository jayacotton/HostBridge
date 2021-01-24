Host Bridge.

Yet Another Interface protocol.

This implementation is wrapped around the Z80PIO to ATMEGA328, or I/O expander chip.

The idea is to creat a data interface that does not require the user to manage the
details of the interface.  So, implemtation hiding.

The code has a few nodes.

NODE SendData, this is obvious (it sends data to the other end of the connection).  
You can call it with a pointer to bytes and a count of bytes to send.  Because of the
design of this code, its best to send a buffer of data i.e. not 1 byte at a time, althou
it will work with that, its just not fast.

MODE ReceiveData, this is an ISR driven service routine, that the user never needs to
call.  It will get an interrupt and put a byte into a data buffer.  It will only put
what it gets, but the sender needs to include an EOM marker when done sending.

Due to the nature of this code, the user must read out the data buffer to a local copy
befor releasing the other host to send more data.  Basically, send a READY to the
other host.

So, ReceiveData will clear the READY flag, when the first byte shows up from the
other host machine.  This allows some gateing of the data flow.  ReceiveData must be
told when the buffer is empty, so it can set the READY flag....  a service entry.
Must also reset the pointers.  (or maybe we use a ring buffer (LIST code)..

Routine GetData will copy the buffer from ReceiveData upto the last byte received, and
return.  If you want to get a full buffer in one read, you will have to poll for the
EOM byte.  (a data wrapper).

Routine SendReady will send a ready signal to the other host (and set up the ReceiveData pointers).

Routine PutData will write a buffer of data to the sending code.  This is asyncronis, so the
sender will hang until the buffer is sent.  It also will send an EOM after it sees the NULL
at the end of the message, or if the input count hits zero.


We will have an implementation in z88dk C compiler.
Macro assembly and C++ (.ina file) for arduino.


