


README for homework06. Includes instructions for server file and the configuration details.


For CONFIGURATION DETAILS:

the CONFIGs are detailed in arch/arm/mm/Kconfig, as seen below, as boolean types
//---------------------------------------------------------------//
# ARMv6
config CPU_V6
	bool "Support ARM V6 processor" if ARCH_INTEGRATOR || MACH_REALVIEW_EB || MACH_REALVIEW_PBX
	select CPU_32v6
	select CPU_ABRT_EV6
	select CPU_CACHE_V6
	select CPU_CACHE_VIPT
	select CPU_COPY_V6 if MMU
	select CPU_CP15_MMU
	select CPU_HAS_ASID if MMU
	select CPU_PABRT_V6
	select CPU_TLB_V6 if MMU

# ARMv6k
config CPU_V6K
	bool "Support ARM V6K processor" if ARCH_INTEGRATOR || MACH_REALVIEW_EB || MACH_REALVIEW_PBX
	select CPU_32v6
	select CPU_32v6K
	select CPU_ABRT_EV6
	select CPU_CACHE_V6
	select CPU_CACHE_VIPT
	select CPU_COPY_V6 if MMU
	select CPU_CP15_MMU
	select CPU_HAS_ASID if MMU
	select CPU_PABRT_V6
	select CPU_TLB_V6 if MMU

# ARMv7
config CPU_V7
	bool "Support ARM V7 processor" if ARCH_INTEGRATOR || MACH_REALVIEW_EB || MACH_REALVIEW_PBX
	select CPU_32v6K
	select CPU_32v7
	select CPU_ABRT_EV7
	select CPU_CACHE_V7
	select CPU_CACHE_VIPT
	select CPU_COPY_V6 if MMU
	select CPU_CP15_MMU
	select CPU_HAS_ASID if MMU
	select CPU_PABRT_V7
	select CPU_TLB_V7 if MMU
//--------------------------------------------------------------------//

in the head.S file under arch/arm/kernel, the b secondary_start_kernel is located at line 389:
//----------------------------------------------------------------------//
NTRY(__secondary_switched)
	ldr	sp, [r7, #4]			@ get secondary_data.stack
	mov	fp, #0
	b	secondary_start_kernel
ENDPROC(__secondary_switched)
//------------------------------------------------------------------------//
main.c itself is located at init/main.c

