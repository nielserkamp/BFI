#ifndef TEPHON_LIVENESS_ARRAY_H
#define TEPHON_LIVENESS_ARRAY_H

#include <udis86/udis86.h>

namespace Tephon {
	enum operand_liveliness_t {
		LL_READ, LL_WRITE,
		LL_READ_WRITE, LL_NONE
	};

	struct insn_liveliness_t {
		ud_mnemonic_code mnemonic;
		operand_liveliness_t type[3];
	};

	extern const insn_liveliness_t operand_liveliness[];
	extern const char *reg_names[];
}

#endif
