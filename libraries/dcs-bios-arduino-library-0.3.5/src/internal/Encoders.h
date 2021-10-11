#ifndef __DCSBIOS_ENCODERS_H
#define __DCSBIOS_ENCODERS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {
	enum StepsPerDetent {
		ONE_STEP_PER_DETENT = 1,
		TWO_STEPS_PER_DETENT = 2,
		FOUR_STEPS_PER_DETENT = 4,
		EIGHT_STEPS_PER_DETENT = 8,
	};

	template <unsigned long pollIntervalMs = POLL_EVERY_TIME, StepsPerDetent stepsPerDetent = ONE_STEP_PER_DETENT>
	class RotaryEncoderT : PollingInput {
	private:
		const char* msg_;
		const char* decArg_;
		const char* incArg_;
		char pinA_;
		char pinB_;
		char lastState_;
		signed char delta_;
		char readState() {
			return (digitalRead(pinA_) << 1) | digitalRead(pinB_);
		}
		void resetState()
		{
			lastState_ = (lastState_==0)?-1:0;
		}
		void pollInput() {
			char state = readState();
			switch(lastState_) {
				case 0:
					if (state == 2) delta_--;
					if (state == 1) delta_++;
					break;
				case 1:
					if (state == 0) delta_--;
					if (state == 3) delta_++;
					break;
				case 2:
					if (state == 3) delta_--;
					if (state == 0) delta_++;
					break;
				case 3:
					if (state == 1) delta_--;
					if (state == 2) delta_++;
					break;
			}
			lastState_ = state;
			
			if (delta_ >= stepsPerDetent) {
				if (tryToSendDcsBiosMessage(msg_, incArg_))
					delta_ -= stepsPerDetent;
			}
			if (delta_ <= -stepsPerDetent) {
				if (tryToSendDcsBiosMessage(msg_, decArg_))
					delta_ += stepsPerDetent;
			}
		}
	public:
		RotaryEncoderT(const char* msg, const char* decArg, const char* incArg, char pinA, char pinB) :
			PollingInput(pollIntervalMs) {
			msg_ = msg;
			decArg_ = decArg;
			incArg_ = incArg;
			pinA_ = pinA;
			pinB_ = pinB;
			pinMode(pinA_, INPUT_PULLUP);
			pinMode(pinB_, INPUT_PULLUP);
			delta_ = 0;
			lastState_ = readState();
		}

		void SetControl( const char* msg )
		{
			msg_ = msg;
		}
	};
	typedef RotaryEncoderT<> RotaryEncoder;

	template <unsigned long pollIntervalMs = POLL_EVERY_TIME, StepsPerDetent stepsPerDetent = ONE_STEP_PER_DETENT>
	class RotaryAcceleratedEncoderT : PollingInput {
    private:
		const char* msg_;
		const char* decArg_;
		const char* incArg_;
		const char* fastDecArg_;
		const char* fastIncArg_;
		char pinA_;
		char pinB_;
		char lastState_;
		signed char delta_;
		char cw_momentum_;

		const unsigned long FAST_THRESHOLD_MS=175;
		const unsigned long STOPPED_THRESHOLD_MS=500;
		const char MAX_MOMENTUM=4;

		unsigned long timeLastDetent_;
		
		char readState() {
			return (digitalRead(pinA_) << 1) | digitalRead(pinB_);
		}
		
		void resetState()
		{
			lastState_ = (lastState_==0)?-1:0;
		}

		void pollInput()
		{
			char state = readState();
			char dir=0;
			switch(lastState_) {
				case 0:
					if (state == 2) dir=-1;
					if (state == 1) dir=+1;
				break;
				case 1:
					if (state == 0) dir=-1;
					if (state == 3) dir=+1;
				break;
				case 2:
					if (state == 3) dir=-1;
					if (state == 0) dir=+1;
				break;
				case 3:
					if (state == 1) dir=-1;
					if (state == 2) dir=+1;
				break;
			}
			lastState_ = state;

			if( dir > 0 )
			{
				// Clockwise
				if( cw_momentum_ >= 0 )
				{
					if( cw_momentum_ < MAX_MOMENTUM*stepsPerDetent )
						cw_momentum_++;
				}
				else
				{
				
					dir = 0;	// Ignore the blip in the "wrong" direction, but reduce the momentum
					cw_momentum_++;
				}
			}
			else if( dir < 0 )
			{
				// Counter-Clockwise
				if( cw_momentum_ <= 0 )
				{
					if( cw_momentum_ > -MAX_MOMENTUM*stepsPerDetent )
						cw_momentum_--;
				}
				else
				{
					dir = 0;	// Ignore the blip in the "wrong" direction, but reduce the momentum
					cw_momentum_--;
				}
			}else{
				if( (millis() - timeLastDetent_) > STOPPED_THRESHOLD_MS )
				cw_momentum_ = 0;
			}

			delta_ += dir;			
			
			if (delta_ >= stepsPerDetent) {
				const char *arg;
				if( (millis() - timeLastDetent_) < FAST_THRESHOLD_MS )
					arg = fastIncArg_;
				else
					arg = incArg_;
				if (tryToSendDcsBiosMessage(msg_, arg))
				{
					delta_ -= stepsPerDetent;
					//delta_ = 0;
					timeLastDetent_ = millis();
				}
			}
			else if (delta_ <= -stepsPerDetent) {
				const char *arg;
				if( (millis() - timeLastDetent_) < FAST_THRESHOLD_MS )
					arg = fastDecArg_;
				else
					arg = decArg_;
				if (tryToSendDcsBiosMessage(msg_, arg))
				{
					delta_ += stepsPerDetent;
					//delta_ = 0;
					timeLastDetent_ = millis();
				}
			}
		}
	public:
		RotaryAcceleratedEncoderT(const char* msg, const char* decArg, const char* incArg, const char* fastDecArg, const char* fastIncArg, char pinA, char pinB) :
				PollingInput(pollIntervalMs)
		{
			msg_ = msg;
			decArg_ = decArg;
			incArg_ = incArg;
			fastDecArg_ = fastDecArg;
			fastIncArg_ = fastIncArg;
			pinA_ = pinA;
			pinB_ = pinB;
			pinMode(pinA_, INPUT_PULLUP);
			pinMode(pinB_, INPUT_PULLUP);
			delta_ = 0;
			lastState_ = readState();
			timeLastDetent_ = millis();
			cw_momentum_ = 0;
		}
  };
  typedef RotaryAcceleratedEncoderT<> RotaryAcceleratedEncoder;
}

#endif
