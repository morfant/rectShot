(
//SynthDef
SynthDef(\bufRead,
	{| freq = 1, bufnum = 0, gate = 1, amp = 1, pan = 0, rate = 1, loop = 1, trig = 1 |
		var sig;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, trigger: trig, loop: loop, doneAction: 2);
	Out.ar(0, Pan2.ar(sig, pan, amp));
}).add;
) //Synthdef END



(// Audio buffers

~mainMixBuf = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/MIX.wav");

~voiceBuf = Array.newClear(20);
~voiceBuf[0] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/youWillBeShot.wav");
~voiceBuf[1] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/portraitRightWarning.wav");
~voiceBuf[2] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/publicService.wav");
~voiceBuf[3] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/underBus.wav");
~voiceBuf[4] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/mySon.wav");
~voiceBuf[5] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/dogChild2.wav");
~voiceBuf[6] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/dogChild.wav");
~voiceBuf[7] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/theyAreWatching.wav");
)


//Main
(
//Oscfuncs
var mainMixSyn, voiceSyn,
ofAddr, oscfn, titleBuf, soundBuf, buf, syn, synFrag, synBlobs, len, lenDiv, lenDivSec,
fragContNum,
maxBox, mainAmpLimit;

maxBox = 80;
mainAmpLimit = 0.6;

fragContNum = Array.newClear(10);
10.do{|i| fragContNum[i] = 0};

ofAddr = NetAddr("127.0.0.1", 20000); // oF addr

oscfn = Array.newClear(10);

voiceSyn = Array.newClear(20);
// voiceBuf = Array.newClear(20);

soundBuf = Array.newClear(10);
buf = Array.newClear(10);
// syn = Array.newClear(10);
syn = Array2D.new(10, 10);
synFrag = Array.newClear(100);
synBlobs = Array.newClear(20);
len = Array.newClear(6);
lenDiv = Array.newClear(6);
lenDivSec = Array.newClear(6);





// titleBuf = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/title.wav");
// buf[0] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/aya.wav");
// buf[1] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/han.wav");
// buf[2] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/sewol.wav");
// buf[3] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/park.wav");


mainMixSyn = Synth(\bufRead, [\bufnum, ~mainMixBuf.bufnum, \amp, 0]);

voiceSyn[0] = Synth(\bufRead, [\bufnum, ~voiceBuf[0].bufnum, \trig, 0, \rate, 0]);
voiceSyn[1] = Synth(\bufRead, [\bufnum, ~voiceBuf[1].bufnum, \trig, 0, \rate, 0]);
voiceSyn[2] = Synth(\bufRead, [\bufnum, ~voiceBuf[2].bufnum, \trig, 0, \rate, 0]);
voiceSyn[3] = Synth(\bufRead, [\bufnum, ~voiceBuf[3].bufnum, \trig, 0, \rate, 0]);
voiceSyn[4] = Synth(\bufRead, [\bufnum, ~voiceBuf[4].bufnum, \trig, 0, \rate, 0]);
voiceSyn[5] = Synth(\bufRead, [\bufnum, ~voiceBuf[5].bufnum, \trig, 0, \rate, 0]);
voiceSyn[6] = Synth(\bufRead, [\bufnum, ~voiceBuf[6].bufnum, \trig, 0, \rate, 0]);
voiceSyn[7] = Synth(\bufRead, [\bufnum, ~voiceBuf[7].bufnum, \trig, 0, \rate, 0]);




//MainMix
oscfn[0] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	//msg[1] : number of box

	var mainAmp = msg[1]/maxBox;
	if (mainAmp >= mainAmpLimit, {mainAmp = mainAmpLimit;}); //Maximum amp limit
	("mainAmp: " + mainAmp).postln;

	mainMixSyn.set(\amp, mainAmp);

}, '/boxNum');


oscfn[1] = OSCFunc({|msg, time, addr, recvPort|

    var idx;
	msg.postln;

	//msg[1] : box idx
	//msg[2] : true/false
    idx = msg[1] % 8;
	("idx: " + idx).postln;

	if (msg[2] == 1,
		{
			voiceSyn[idx].set(\rate, 1, \trig, 1);
		},
        {
			voiceSyn[idx].set(\rate, 0);
        });


}, '/canHit');

oscfn[2] = OSCFunc({|msg, time, addr, recvPort|

    var idx;
	msg.postln;

	//msg[1] : box idx
    idx = msg[1] % 8;

	voiceSyn[idx].set(\rate, 0);

}, '/brkBox');



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

	20.do{|i| voiceSyn[i].free;};



}


)