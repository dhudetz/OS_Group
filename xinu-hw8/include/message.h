#ifndef _MESSAGE_H_
#define _MESSAGE_H_

message recv(void);
message recvnow(void);
syscall send(int pid, message msg);
syscall sendnow(int pid, message msg);


#endif                          /* _MESSAGE_H_ */
