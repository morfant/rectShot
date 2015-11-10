o = OSCFunc({|msg, time, addr, recvPort| msg.postln}, '/xbee');
o.free;



(
SynthDef.new(\di,
	{
		arg freq = 50, mf = 10, cf = 10, pan = 0, amp = 0.4, freq2 = 10;
		var sig = SinOsc.ar(freq, mul: amp);
		Out.ar(0, Pan2.ar(sig, pan, 1));
	}
).add;

SynthDef.new(\ti,
	{
		arg freq = 50, amfreq = 10, panfreq = 1/10, amp = 0.4, gate = 1;
		var sig, amsig, env, pan;
		amsig = SinOsc.kr(amfreq);
		sig = SinOsc.ar(freq, mul: amsig);
		env = EnvGen.kr(Env.perc(0.01, 0.2), gate, doneAction: 2);
		Out.ar(0, Pan2.ar(sig*env, 0, amp));
	}
).add;

SynthDef.new(\ai,
	{
		arg freq = 50, panfreq = 1/10, amp = 0.4, gate = 1, rq = 0.05;
		var sig, env, pan, fil;
		sig = Dust.ar(10, amp);
		fil = RLPF.ar(sig, freq, rq);
		env = EnvGen.kr(Env.adsr, gate, doneAction: 2);
		Out.ar(0, Pan2.ar(fil*env, 0, amp));
	}
).add;

)

1.asDigit
1.asAscii
'1'.asDigits

2007.asDigits(10);

(
a = Synth(\di);
//b = Synth(\ti);

p = OSCFunc({|msg, time, addr, recvPort|
	var v = msg[2].asInteger.abs;
	var m = msg[3].asInteger.abs;
	var g = msg[4].asInteger.abs;
//	msg[1].class.postln;
	("v: " + v).postln;
	("m: " + m).postln;
	("g: " + g).postln;

	a.set(\freq, 1 + ((v)*10));


}, '/test');
)

p.free;

q.free;q = OSCFunc({|msg, time, addr, recvPort| msg.postln;}, '/fromOF_Ball');


// Set Synthes with blobs info.(position, area)
(
var syn, oscf;
syn = Array.newClear(10);

for(0, 9, {|i|
	syn[i] = Synth(\di, [\amp, 0])
});

oscf.free;oscf = OSCFunc({|msg, time, addr, recvPort|
//	msg.postln;
	// for(0, 9, {|i|
	syn[msg[1].asInteger].set(
			\freq, (480 - msg[3]),
			\amp, msg[4].linlin(0, 200000, 0, 1),
//			\freq2, msg[4].linlin(0, 200000, 2, 200),
			\pan, msg[2].linlin(0, 720, -1, 1)
		);
//		};
//	);
}, '/fromOF')
);




(
var syn, oscf;

syn = Array.newClear(10);
// for(0, 9, {|i|
// 	syn[i] = Synth(\ti, [\amp, 0])
// });

oscf.free;oscf = OSCFunc({|msg, time, addr, recvPort|
//	msg.postln;
	// for(0, 9, {|i|
	syn[0] = Synth(\ti, [\freq, msg[1].linlin(0, 1024, 80, 2000), \amfreq, msg[2].linlin(768, 0, 1, 30)]);
//		};
//	);
}, '/fromOF_Ball')
);



( // Pbody pointer.
var syn, oscf;

syn = Array.newClear(10);
for(0, 9, {|i|
	syn[i] = Synth(\ai, [\amp, 0])
});

oscf.free;oscf = OSCFunc({|msg, time, addr, recvPort|
//	msg.postln;
	if ((msg[2].asInteger == -1),{
		("kill " + msg[1] + " : " + msg[2]).postln;
		syn[msg[1].asInteger].set(\gate, 0);
		},{
			syn[msg[1].asInteger].set(
			\gate, 1,
			\amp, 0.4,
			\freq, msg[2].linlin(0, 1024, 50, 3000),
			\rq, msg[3].linlin(0, 769, 0, 1));
	});
}, '/fromOF_pBody')
);
