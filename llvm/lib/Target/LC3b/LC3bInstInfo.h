#define GET_INSTRINFO_HEADER
#include "LC3bGenInstrInfo.inc"

class LC3bInstrInfo : public LC3bGenInstrInfo {
	LC3bTargetMachine &TM;
public:
	explicit LC3bInstrInfo(LC3bTargetMachine &TM);
};
