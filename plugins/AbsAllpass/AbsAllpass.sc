AbsAllpass : UGen {
	*ar { |
        in = 0.0,
        maxDelayTime = 0.2,
        delayTime = 0.2,
        coefficient = 0.7,
        filterFreq = 1000
        |
		^this.multiNew('audio', in.asAudioRateInput(this), maxDelayTime, delayTime, coefficient, filterFreq).madd(mul, add);
	}
	checkInputs {
	    if(this.rate == \audio and: { inputs.ar(0).rate != \audio}, {
            Error(
                "AbsAllpass expects an audio rate signal";
            ).throw;
        };
		^this.checkValidInputs;
	}
}
