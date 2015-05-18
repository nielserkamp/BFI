#include "LivenessArray.h"
namespace Tephon {
const insn_liveliness_t operand_liveliness[] = {
		{UD_I3dnow, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaaa, 			{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaad, 			{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaam, 			{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaas, 			{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iadc, 			{LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Iadd, 			{LL_READ_WRITE, 	LL_READ, 		LL_NONE}},
		{UD_Iaddpd, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaddps, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaddsd, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaddss, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaddsubpd, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iaddsubps, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iand, 			{LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Iandpd, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iandps, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iandnpd, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iandnps, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iarpl, 			{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovsxd, 		{LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Ibound, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ibsf, 			{LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Ibsr, 			{LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Ibswap, 		{LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ibt, 			{LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_Ibtc, 			{LL_READ_WRITE, 	LL_READ,  		LL_NONE}},
		{UD_Ibtr, 			{LL_READ_WRITE, 	LL_READ,  		LL_NONE}},
		{UD_Ibts, 			{LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Icall, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Icbw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icwde, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icdqe, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iclc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icld, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iclflush, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iclgi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icli, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iclts, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmovo, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovno, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovb, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovae, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovz, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovnz, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovbe, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmova, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovs, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovns, { 		LL_WRITE,  		LL_READ, 	 	LL_NONE}},
		{UD_Icmovp, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovnp, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovl, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovge, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovle, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmovg, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Icmp, { 		LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_Icmppd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpsb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpsq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icmpxchg, { LL_READ_WRITE,  LL_READ_WRITE,  LL_NONE}},
		{UD_Icmpxchg8b, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icomisd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icomiss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icpuid, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtdq2pd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtdq2ps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtpd2dq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtpd2pi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtpd2ps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtpi2ps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtpi2pd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtps2dq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtps2pi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtps2pd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtsd2si, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtsd2ss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtsi2ss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtss2si, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtss2sd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvttpd2pi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvttpd2dq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvttps2dq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvttps2pi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvttsd2si, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvtsi2sd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icvttss2si, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icwd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icdq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Icqo, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idaa, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idas, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idec, { 		LL_READ_WRITE,  LL_NONE,  		LL_NONE}},
		{UD_Idiv, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Idivpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idivps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idivsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idivss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iemms, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ienter, { 		LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_If2xm1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifabs, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifadd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifaddp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifbld, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifbstp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifchs, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifclex, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmove, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovbe, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovu, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovnb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovne, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovnbe, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcmovnu, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifucomi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcom, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcom2, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcomp3, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcomi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifucomip, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcomip, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcomp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcomp5, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcompp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifcos, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifdecstp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifdiv, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifdivp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifdivr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifdivrp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifemms, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iffree, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iffreep, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ificom, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ificomp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifild, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifncstp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifninit, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifiadd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifidivr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifidiv, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifisub, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifisubr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifist, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifistp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifisttp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifld, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifld1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldl2t, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldl2e, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldlpi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldlg2, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldln2, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldz, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldcw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifldenv, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifmul, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifmulp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifimul, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifnop, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifpatan, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifprem, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifprem1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifptan, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifrndint, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifrstor, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifnsave, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifscale, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsin, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsincos, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsqrt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifstp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifstp1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifstp8, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifstp9, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifst, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifnstcw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifnstenv, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifnstsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsub, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsubp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsubr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifsubrp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iftst, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifucom, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifucomp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifucompp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifxam, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifxch, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifxch4, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifxch7, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifxrstor, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifxsave, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifpxtract, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifyl2x, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ifyl2xp1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ihaddpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ihaddps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ihlt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ihsubpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ihsubps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iidiv, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iin, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iimul, { 		LL_READ_WRITE,  LL_READ,  		LL_READ}},
		{UD_Iinc, { 		LL_READ_WRITE,  LL_NONE,  		LL_NONE}},
		{UD_Iinsb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iinsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iinsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iint1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iint3, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iint, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Iinto, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iinvd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iinvlpg, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iinvlpga, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iiretw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iiretd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iiretq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ijo, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijno, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijb, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijae, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijz, { 			LL_READ, 	 	LL_NONE,  		LL_NONE}},
		{UD_Ijnz, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijbe, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ija, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijs, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijns, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijp, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijnp, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijl, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijge, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijle, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijg, { 			LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ijcxz, { 		LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_Ijecxz, { 		LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_Ijrcxz, { 		LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_Ijmp, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ilahf, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilar, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilddqu, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ildmxcsr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilds, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilea, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Iles, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilfs, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilgs, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilidt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ileave, { 		LL_NONE,  		LL_NONE,  		LL_NONE}},
		{UD_Ilfence, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilgdt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Illdt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilmsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilock, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilodsb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilodsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilodsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilodsq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iloopnz, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iloope, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iloop, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ilsl, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iltr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imaskmovq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imaxpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imaxps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imaxsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imaxss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imfence, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iminpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iminps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iminsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iminss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imonitor, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imov, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovapd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovaps, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovd, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovddup, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovdqa, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovdqu, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovdq2q, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovhpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovhps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovlhps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovlpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovlps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovhlps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovmskpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovmskps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovntdq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovnti, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovntpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovntps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovntq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovq, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovqa, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovq2dq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovsb, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovsw, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovsd, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovsq, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovsldup, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovshdup, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovss, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovsx, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Imovupd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovups, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imovzx, { 		LL_WRITE, 	 	LL_READ,  		LL_NONE}},
		{UD_Imul, { 		LL_READ_WRITE,  LL_NONE,  		LL_NONE}},
		{UD_Imulpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imulps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imulsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imulss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Imwait, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ineg, { 		LL_READ_WRITE,  LL_NONE,  		LL_NONE}},
		{UD_Inop, { 		LL_NONE,  		LL_NONE,  		LL_NONE}},
		{UD_Inot, { 		LL_READ_WRITE,  LL_NONE,  		LL_NONE}},
		{UD_Ior, {			LL_READ_WRITE,  LL_READ, 		LL_NONE}},
		{UD_Iorpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iorps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iout, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ioutsb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ioutsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ioutsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ioutsq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipacksswb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipackssdw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipackuswb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddsb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddusb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipaddusw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipand, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipandn, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipause, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipavgb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipavgw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipcmpeqb, { 		LL_READ_WRITE,  LL_READ,  	LL_NONE}},
		{UD_Ipcmpeqw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipcmpeqd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipcmpgtb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipcmpgtw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipcmpgtd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipextrw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipinsrw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmaddwd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmaxsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmaxub, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipminsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipminub, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmovmskb, { 	LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Ipmulhuw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmulhw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmullw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmuludq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipop, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Ipopa, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Ipopad, { LL_WRITE,  LL_NONE,  LL_NONE}},
		{UD_Ipopfw, { LL_WRITE,  LL_NONE,  LL_NONE}},
		{UD_Ipopfd, { LL_WRITE,  LL_NONE,  LL_NONE}},
		{UD_Ipopfq, { LL_WRITE,  LL_NONE,  LL_NONE}},
		{UD_Ipor, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iprefetch, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iprefetchnta, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iprefetcht0, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iprefetcht1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iprefetcht2, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsadbw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipshufd, { 		LL_WRITE,  		LL_READ,  		LL_READ}},
		{UD_Ipshufhw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipshuflw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipshufw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipslldq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsllw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipslld, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsllq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsraw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsrad, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsrlw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsrld, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsrlq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsrldq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubsb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubusb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipsubusw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpckhbw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpckhwd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpckhdq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpckhqdq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpcklbw, { 		LL_READ_WRITE,  LL_READ,  	LL_NONE}},
		{UD_Ipunpcklwd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpckldq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipunpcklqdq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipi2fw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipi2fd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipf2iw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipf2id, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfnacc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfpnacc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfcmpge, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfmin, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfrcp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfrsqrt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfsub, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfadd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfcmpgt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfmax, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfrcpit1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfrspit1, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfsubr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfacc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfcmpeq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfmul, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipfrcpit2, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipmulhrw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipswapd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipavgusb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ipush, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ipusha, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Ipushad, { LL_READ,  LL_NONE,  LL_NONE}},
		{UD_Ipushfw, { LL_READ,  LL_NONE,  LL_NONE}},
		{UD_Ipushfd, { LL_READ,  LL_NONE,  LL_NONE}},
		{UD_Ipushfq, { LL_READ,  LL_NONE,  LL_NONE}},
		{UD_Ipxor, { 		LL_READ_WRITE,  	LL_READ,  		LL_NONE}},
		{UD_Ircl, { 		LL_NONE,  		LL_NONE,  		LL_NONE}},
		{UD_Ircr, { 		LL_NONE,  		LL_NONE,  		LL_NONE}},
		{UD_Irol, { 		LL_READ_WRITE,  	LL_READ,  		LL_NONE}},
		{UD_Iror, { 		LL_READ_WRITE,  	LL_READ,  		LL_NONE}},
		{UD_Ircpps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ircpss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irdmsr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irdpmc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irdtsc, { 		LL_NONE,  		LL_NONE,  		LL_NONE}},
		{UD_Irdtscp, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irepne, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irep, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iret, { 		LL_READ,  		LL_NONE,  		LL_NONE}},
		{UD_Iretf, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irsm, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irsqrtps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Irsqrtss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isahf, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isal, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Isalc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isar, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Ishl, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Ishr, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Isbb, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Iscasb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iscasw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iscasd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iscasq, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iseto, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetno, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetb, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetnb, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetz, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetnz, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetbe, { 		LL_WRITE,  		LL_NONE, 	 	LL_NONE}},
		{UD_Iseta, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isets, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetns, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetp, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetnp, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetl, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetge, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetle, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isetg, { 		LL_WRITE,  		LL_NONE,  		LL_NONE}},
		{UD_Isfence, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isgdt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ishld, { 		LL_READ_WRITE,  LL_READ,  		LL_READ}},
		{UD_Ishrd, { 		LL_READ_WRITE,  LL_READ,  		LL_READ}},
		{UD_Ishufpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ishufps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isidt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isldt, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ismsw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isqrtps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isqrtpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isqrtsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isqrtss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Istc, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Istd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Istgi, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isti, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iskinit, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Istmxcsr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Istosb, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Istosw, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Istosd, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Istosq, { 		LL_WRITE,  		LL_READ,  		LL_NONE}},
		{UD_Istr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isub, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Isubpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isubps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isubsd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isubss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iswapgs, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isyscall, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isysenter, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isysexit, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Isysret, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Itest, { 		LL_READ,  		LL_READ,  		LL_NONE}},
		{UD_Iucomisd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iucomiss, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iud2, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iunpckhpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iunpckhps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iunpcklps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iunpcklpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iverr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iverw, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmcall, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmclear, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmxon, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmptrld, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmptrst, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmresume, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmxoff, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmrun, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmmcall, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmload, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ivmsave, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iwait, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iwbinvd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iwrmsr, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ixadd, { 		LL_READ_WRITE,  LL_READ_WRITE,  LL_NONE}},
		{UD_Ixchg, { 		LL_READ_WRITE,  LL_READ_WRITE,  LL_NONE}},
		{UD_Ixlatb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ixor, { 		LL_READ_WRITE,  LL_READ,  		LL_NONE}},
		{UD_Ixorpd, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ixorps, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Idb, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Iinvalid, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Id3vil, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Ina, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_reg, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_rm, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_vendor, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_x87, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_mode, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_osize, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_asize, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Igrp_mod, { LL_NONE,  LL_NONE,  LL_NONE}},
		{UD_Inone, {LL_NONE, LL_NONE, LL_NONE}}
	};
	
	const char *reg_names[] = {
  "NONE",

  /* 8 bit GPRs */
  "AL",	"CL",	"DL",	"BL",
  "AH",	"CH",	"DH",	"BH",
  "SPL",	"BPL",	"SIL",	"DIL",
  "R8B",	"R9B",	"R10B",	"R11B",
  "R12B",	"R13B",	"R14B",	"R15B",

  /* 16 bit GPRs */
  "AX",	"CX",	"DX",	"BX",
  "SP",	"BP",	"SI",	"DI",
  "R8W",	"R9W",	"R10W",	"R11W",
  "R12W",	"R13W",	"R14W",	"R15W",
	
  /* 32 bit GPRs */
  "EAX",	"ECX",	"EDX",	"EBX",
  "ESP",	"EBP",	"ESI",	"EDI",
  "R8D",	"R9D",	"R10D",	"R11D",
  "R12D",	"R13D",	"R14D",	"R15D",
	
  /* 64 bit GPRs */
  "RAX",	"RCX",	"RDX",	"RBX",
  "RSP",	"RBP",	"RSI",	"RDI",
  "R8",	"R9",	"R10",	"R11",
  "R12",	"R13",	"R14",	"R15",

  /* segment registers */
  "ES",	"CS",	"SS",	"DS",
  "FS",	"GS",	

  /* control registers*/
  "CR0",	"CR1",	"CR2",	"CR3",
  "CR4",	"CR5",	"CR6",	"CR7",
  "CR8",	"CR9",	"CR10",	"CR11",
  "CR12",	"CR13",	"CR14",	"CR15",
	
  /* debug registers */
  "DR0",	"DR1",	"DR2",	"DR3",
  "DR4",	"DR5",	"DR6",	"DR7",
  "DR8",	"DR9",	"DR10",	"DR11",
  "DR12",	"DR13",	"DR14",	"DR15",

  /* mmx registers */
  "MM0",	"MM1",	"MM2",	"MM3",
  "MM4",	"MM5",	"MM6",	"MM7",

  /* x87 registers */
  "ST0",	"ST1",	"ST2",	"ST3",
  "ST4",	"ST5",	"ST6",	"ST7", 

  /* extended multimedia registers */
  "XMM0",	"XMM1",	"XMM2",	"XMM3",
  "XMM4",	"XMM5",	"XMM6",	"XMM7",
  "XMM8",	"XMM9",	"XMM10",	"XMM11",
  "XMM12",	"XMM13",	"XMM14",	"XMM15",

  "RIP",

  /* Operand Types */
  "REG",	"MEM",	"PTR",	"IMM",	
  "JIMM",	"CONST"
};
}
