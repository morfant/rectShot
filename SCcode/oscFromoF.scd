Lone Wolf, 2015.12.20

(
//SynthDef
SynthDef(\bufRead,
	{| freq = 1, bufnum = 0, gate = 1, amp = 0.1, pan = 0, rate = 1, loop = 1 |
		var sig;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, loop: loop, doneAction: 2);
	Out.ar(0, Pan2.ar(sig, pan, amp));
}).add;


SynthDef(\bufFrag,
	{| freq = 1, bufnum = 0, gate = 1, amp = 0.3, pan = 0, rate = 1, trig = 1, loop = 1, sp = 0, len = 0 |
		var sig, env;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, trig, sp, loop: loop);
		env = EnvGen.kr(Env.linen(0.01, len-0.02, 0.01), gate, doneAction: 2);
	Out.ar(0, Pan2.ar(sig*env, pan, amp));
}).add;


SynthDef(\bufFrag2,
	{| freq = 1, bufnum = 0, gate = 1, amp = 0.01, pan = 0, rate = 1, trig = 1, loop = 1, sp = 0, len = 0 |
		var sig, env;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, trig, sp, loop: loop);
		env = EnvGen.kr(Env.linen(0.01, len-0.02, 0.01), gate, doneAction: 2);
	Out.ar(0, Pan2.ar(sig*env, pan, amp));
}).add;

SynthDef(\bufFragLoop,
	{| freq = 1, bufnum = 0, gate = 1, amp = 0.1, pan = 0,
		rate = 1, trig = 1, loop = 1, sp = 0, len = 0 |
		var sig;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, Impulse.kr(freq), sp, loop: loop);
	Out.ar(0, Pan2.ar(sig, pan, amp));
}).add;


SynthDef(\liedR,
    {
        |freq = 2598, amp = 0.3, gate = 1, efxfreq = 700, panfreq = 0.1, dronerq = 0.4, pan = 0|
        var sig, env, efx, pansig, out, delay, drone;
        sig = Saw.ar(freq, mul: amp);
        env = EnvGen.kr(Env.adsr(0.1, 0.2, 0.5, 2, 1), gate, doneAction: 2);
        efx = RLPF.ar(sig*env, efxfreq, 0.2, 1);
        drone = RLPF.ar(BrownNoise.ar(0.01), 120, dronerq, 1);
        delay = CombC.ar(efx+drone, 0.2, 0.1, 0.03, 1, efx);
		env = EnvGen.kr(Env.asr(0.1, 0.2, 0.5), gate, doneAction: 2);

        pansig = LFTri.kr(panfreq, mul: 0.5, add: 0.25);
        out = Out.ar(0, Pan2.ar(delay, pan));
    }).add;


SynthDef(\filmPan,{
	arg freq = 1, ffreq = 100, rq = 0.05, amp = 0.6, pan = 0, gate = 1, out = 0;
	var sig1, sig2, sig3, sigsum, fil, env;
	sig1 = Impulse.ar(freq);
	sig2 = LFPulse.ar(freq, 0.1);
	sig3 = Pulse.ar(freq, 0.3, mul: 0.01);
	sigsum = sig1 + sig2 + sig3;
	fil = BPF.ar(sigsum, ffreq, rq, add: sigsum * 0.7);
	env = EnvGen.kr(Env.perc(0.01, 0.2), gate, doneAction: 2);
	Out.ar(out, Pan2.ar(fil * env, pan, amp));

	}).add;

)


(
//Oscfuncs
var ofAddr, oscfn, titleBuf, soundBuf, buf, syn, synFrag, synBlobs, len, lenDiv, lenDivSec,
fragContNum;

fragContNum = Array.newClear(10);
10.do{|i| fragContNum[i] = 0};

ofAddr = NetAddr("127.0.0.1", 20000); // oF addr

oscfn = Array.newClear(10);
soundBuf = Array.newClear(10);
buf = Array.newClear(10);
// syn = Array.newClear(10);
syn = Array2D.new(10, 10);
synFrag = Array.newClear(100);
synBlobs = Array.newClear(20);
len = Array.newClear(6);
lenDiv = Array.newClear(6);
lenDivSec = Array.newClear(6);


// Audio buffers
titleBuf = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/title.wav");
soundBuf[0] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/hafen.wav");
soundBuf[1] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/latoon.wav");
soundBuf[2] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/high.wav");
soundBuf[3] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/woong.wav");
buf[0] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/aya.wav");
buf[1] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/han.wav");
buf[2] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/sewol.wav");
buf[3] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/park.wav");

//Polygon body
oscfn[0] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	//msg[1] : polygonBody idx
	//msg[2] : dupIdx

	syn[msg[1], msg[2]] = Synth(\bufRead, [\bufnum, buf[msg[1]]]);


}, '/pbBorn');


oscfn[1] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	//msg[1] : polygonBody idx
	//msg[2] : dupIdx

	syn[msg[1], msg[2]].free;


}, '/pbBrek');

oscfn[2] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	syn[msg[1], msg[2]].free;

}, '/pbDest');



//Fragments
oscfn[3] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = polygonBodyIdx
	msg[2] = fragIdx
	*/

	if (msg[1] < 4, {

	len[msg[1]] = buf[msg[1]].numFrames;
	lenDiv[msg[1]] = len[msg[1]]/100;
	lenDivSec[msg[1]] = lenDiv[msg[1]]/s.sampleRate;

	Synth(\bufFrag, [\bufnum, buf[msg[1]], \amp, 0.1, \sp, msg[2]*lenDiv[msg[1]], \len, lenDivSec[msg[1]]]);
	// synFrag[msg[2]] = Synth(\bufFragLoop, [\bufnum, buf[msg[1]], \sp, msg[2]*lenDiv[msg[1]],
	// \freq, lenDivSec[msg[1]].reciprocal]);
		});

}, '/fgBorn');


oscfn[4] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = polygonBodyIdx
	msg[2] = fragIdx
	*/

	len[msg[1]] = soundBuf[msg[1]].numFrames;
	lenDiv[msg[1]] = len[msg[1]]/100;
	lenDivSec[msg[1]] = lenDiv[msg[1]]/s.sampleRate;

	// synFrag[msg[2]].free;



	// Synth(\bufFrag, [\bufnum, buf[msg[1]], \sp, msg[2]*lenDiv[msg[1]], \len, lenDivSec[msg[1]]]);
	Synth(\bufFrag, [\bufnum, soundBuf[msg[1]], \amp, 0.05, \sp, fragContNum[msg[1]]*lenDiv[msg[1]], \len, lenDivSec[msg[1]]]);
	fragContNum[msg[1]] = fragContNum[msg[1]] + 1;

	if (fragContNum[msg[1]] == 100, {
		fragContNum[msg[1]] = 0;
	});

	// fragContNum[msg[1]].postln;




}, '/fgCont');

oscfn[5] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	if(msg[1] == 1, {
		titleBuf.play;
	});

}, '/title');


oscfn[6] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = blobs order
	msg[2] = centroid.x
	msg[3] = centroid.y
	msg[4] = blobs area
	*/

	synBlobs[msg[1]].set(\freq, msg[3], \pan, msg[2].linlin(0, 1200, -1, 1), \amp, msg[4].linlin(0, 60000, 0.0, 0.2));


}, '/blobs');

oscfn[7] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = blob num
	*/

	if(msg[1] == 0,
		{
			10.do{|i| synBlobs[i].release(2)};
			10.do{|i|10.do{|j| syn[i,j].free;};

			};

		},
		{ msg[1].do{|i| synBlobs[i] = Synth(\liedR, [\amp, 0.1]);};}

	);

}, '/creatBlobSyn');


oscfn[8] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = 0 or 1
	*/

	if(msg[1] == 1,{

		10.do{|i|
			10.do{|j| syn[i,j].free;};

			synBlobs[i].free;
			buf[i].free;
			soundBuf[i].free;
			titleBuf.free;
		};
	});

}, '/blackout');



CmdPeriod.doOnce{
	10.do{|i| oscfn[i].free;};
}


)
