(
o = OSCFunc({|msg, time, addr, recvPort|
	msg.postln;

	//msg[1] : polygonBody idx
	//msg[2] : dupIdx

	// syn[msg[1], msg[2]].free;


}, '/canHit');
);


(
p = OSCFunc({|msg, time, addr, recvPort|
	msg.postln;

	//msg[1] : polygonBody idx
	//msg[2] : dupIdx

	// syn[msg[1], msg[2]].free;


}, '/boxNum');
);