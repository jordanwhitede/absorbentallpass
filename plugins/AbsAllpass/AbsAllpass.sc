AbsAllpass : UGen {
	*ar { |in = 0.0, delayTime = 0.2, coefficient = 0.7, freq = 1000|
		^this.multiNew('audio', in, delayTime, coefficient, freq);
	}
	checkInputs {
		^this.checkValidInputs;
	}
}
