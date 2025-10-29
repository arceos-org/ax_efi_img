/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_RWONCE_H
#define __ASM_GENERIC_RWONCE_H

#define __WRITE_ONCE(x, val)						\
do {									\
	*(volatile typeof(x) *)&(x) = (val);				\
} while (0)

#define WRITE_ONCE(x, val)						\
do {									\
	__WRITE_ONCE(x, val);						\
} while (0)

#endif	/* __ASM_GENERIC_RWONCE_H */
