//FIXME : TO BE COPIED FROM LLVM
class TargetInstrInfo : public MCInstrInfo {
TargetInstrInfo(const TargetInstrInfo &) LLVM_DELETED_FUNCTION;
void operator=(const TargetInstrInfo &) LLVM_DELETED_FUNCTION;
public:
...
}
...
class TargetInstrInfoImpl : public TargetInstrInfo {
protected:
TargetInstrInfoImpl(int CallFrameSetupOpcode = -1,
int CallFrameDestroyOpcode = -1)
: TargetInstrInfo(CallFrameSetupOpcode, CallFrameDestroyOpcode) {}
public:
...
}

