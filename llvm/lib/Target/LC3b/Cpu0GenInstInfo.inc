//- LC3bGenInstInfo.inc which generate from LC3bInstrInfo.td
#ifdef GET_INSTRINFO_HEADER
#undef GET_INSTRINFO_HEADER
namespace llvm {

struct LC3bGenInstrInfo : public TargetInstrInfoImpl {
	explicit LC3bGenInstrInfo(int SO = -1, int DO = -1);
};

} // End llvm namespace

#endif // GET_INSTRINFO_HEADER




