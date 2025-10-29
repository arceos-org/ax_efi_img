/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _DRIVERS_FIRMWARE_EFI_EFISTUB_H
#define _DRIVERS_FIRMWARE_EFI_EFISTUB_H

#include "efi.h"

#define __noreturn  __attribute__((__noreturn__))

extern void efi_puts(const char *str);
extern void efi_put_u64(unsigned long n);
extern void sbi_puts(const char *str);

typedef struct {
	u16 scan_code;
	efi_char16_t unicode_char;
} efi_input_key_t;

typedef void *efi_event_t;
/* Note that notifications won't work in mixed mode */
typedef void (__efiapi *efi_event_notify_t)(efi_event_t, void *);

typedef enum {
    EfiTimerCancel,
    EfiTimerPeriodic,
    EfiTimerRelative
} EFI_TIMER_DELAY;

typedef struct efi_generic_dev_path efi_device_path_protocol_t;

union efi_simple_text_input_protocol {
	struct {
		void *reset;
		efi_status_t (__efiapi *read_keystroke)(efi_simple_text_input_protocol_t *,
							efi_input_key_t *);
		efi_event_t wait_for_key;
	};
	struct {
		u32 reset;
		u32 read_keystroke;
		u32 wait_for_key;
	} mixed_mode;
};

union efi_simple_text_output_protocol {
	struct {
		void *reset;
		efi_status_t (__efiapi *output_string)(efi_simple_text_output_protocol_t *,
						       efi_char16_t *);
		void *test_string;
	};
	struct {
		u32 reset;
		u32 output_string;
		u32 test_string;
	} mixed_mode;
};

typedef union {
	struct {
		u32			revision;
		efi_handle_t		parent_handle;
		efi_system_table_t	*system_table;
		efi_handle_t		device_handle;
		void			*file_path;
		void			*reserved;
		u32			load_options_size;
		void			*load_options;
		void			*image_base;
		__aligned_u64		image_size;
		unsigned int		image_code_type;
		unsigned int		image_data_type;
		efi_status_t		(__efiapi *unload)(efi_handle_t image_handle);
	};
	struct {
		u32		revision;
		u32		parent_handle;
		u32		system_table;
		u32		device_handle;
		u32		file_path;
		u32		reserved;
		u32		load_options_size;
		u32		load_options;
		u32		image_base;
		__aligned_u64	image_size;
		u32		image_code_type;
		u32		image_data_type;
		u32		unload;
	} mixed_mode;
} efi_loaded_image_t;

/*
 * EFI Boot Services table
 */
union efi_boot_services {
	struct {
		efi_table_hdr_t hdr;
		void *raise_tpl;
		void *restore_tpl;
		efi_status_t (__efiapi *allocate_pages)(int, int, unsigned long,
							efi_physical_addr_t *);
		efi_status_t (__efiapi *free_pages)(efi_physical_addr_t,
						    unsigned long);
		efi_status_t (__efiapi *get_memory_map)(unsigned long *, void *,
							unsigned long *,
							unsigned long *, u32 *);
		efi_status_t (__efiapi *allocate_pool)(int, unsigned long,
						       void **);
		efi_status_t (__efiapi *free_pool)(void *);
		efi_status_t (__efiapi *create_event)(u32, unsigned long,
						      efi_event_notify_t, void *,
						      efi_event_t *);
		efi_status_t (__efiapi *set_timer)(efi_event_t,
						  EFI_TIMER_DELAY, u64);
		efi_status_t (__efiapi *wait_for_event)(unsigned long,
							efi_event_t *,
							unsigned long *);
		void *signal_event;
		efi_status_t (__efiapi *close_event)(efi_event_t);
		void *check_event;
		void *install_protocol_interface;
		void *reinstall_protocol_interface;
		void *uninstall_protocol_interface;
		efi_status_t (__efiapi *handle_protocol)(efi_handle_t,
							 efi_guid_t *, void **);
		void *__reserved;
		void *register_protocol_notify;
		efi_status_t (__efiapi *locate_handle)(int, efi_guid_t *,
						       void *, unsigned long *,
						       efi_handle_t *);
		efi_status_t (__efiapi *locate_device_path)(efi_guid_t *,
							    efi_device_path_protocol_t **,
							    efi_handle_t *);
		efi_status_t (__efiapi *install_configuration_table)(efi_guid_t *,
								     void *);
		efi_status_t (__efiapi *load_image)(bool, efi_handle_t,
						    efi_device_path_protocol_t *,
						    void *, unsigned long,
						    efi_handle_t *);
		efi_status_t (__efiapi *start_image)(efi_handle_t, unsigned long *,
						     efi_char16_t **);
		efi_status_t __noreturn (__efiapi *exit)(efi_handle_t,
							 efi_status_t,
							 unsigned long,
							 efi_char16_t *);
		efi_status_t (__efiapi *unload_image)(efi_handle_t);
		efi_status_t (__efiapi *exit_boot_services)(efi_handle_t,
							    unsigned long);
		void *get_next_monotonic_count;
		efi_status_t (__efiapi *stall)(unsigned long);
		void *set_watchdog_timer;
		void *connect_controller;
		efi_status_t (__efiapi *disconnect_controller)(efi_handle_t,
							       efi_handle_t,
							       efi_handle_t);
		void *open_protocol;
		void *close_protocol;
		void *open_protocol_information;
		void *protocols_per_handle;
		void *locate_handle_buffer;
		efi_status_t (__efiapi *locate_protocol)(efi_guid_t *, void *,
							 void **);
		efi_status_t (__efiapi *install_multiple_protocol_interfaces)(efi_handle_t *, ...);
		efi_status_t (__efiapi *uninstall_multiple_protocol_interfaces)(efi_handle_t, ...);
		void *calculate_crc32;
		void (__efiapi *copy_mem)(void *, const void *, unsigned long);
		void (__efiapi *set_mem)(void *, unsigned long, unsigned char);
		void *create_event_ex;
	};
	struct {
		efi_table_hdr_t hdr;
		u32 raise_tpl;
		u32 restore_tpl;
		u32 allocate_pages;
		u32 free_pages;
		u32 get_memory_map;
		u32 allocate_pool;
		u32 free_pool;
		u32 create_event;
		u32 set_timer;
		u32 wait_for_event;
		u32 signal_event;
		u32 close_event;
		u32 check_event;
		u32 install_protocol_interface;
		u32 reinstall_protocol_interface;
		u32 uninstall_protocol_interface;
		u32 handle_protocol;
		u32 __reserved;
		u32 register_protocol_notify;
		u32 locate_handle;
		u32 locate_device_path;
		u32 install_configuration_table;
		u32 load_image;
		u32 start_image;
		u32 exit;
		u32 unload_image;
		u32 exit_boot_services;
		u32 get_next_monotonic_count;
		u32 stall;
		u32 set_watchdog_timer;
		u32 connect_controller;
		u32 disconnect_controller;
		u32 open_protocol;
		u32 close_protocol;
		u32 open_protocol_information;
		u32 protocols_per_handle;
		u32 locate_handle_buffer;
		u32 locate_protocol;
		u32 install_multiple_protocol_interfaces;
		u32 uninstall_multiple_protocol_interfaces;
		u32 calculate_crc32;
		u32 copy_mem;
		u32 set_mem;
		u32 create_event_ex;
	} mixed_mode;
};

extern const efi_system_table_t *efi_system_table;

#define efi_is_native()			(true)
#define efi_table_attr(inst, attr)	(inst)->attr
#define efi_fn_call(inst, func, ...)	(inst)->func(__VA_ARGS__)

#define efi_call_proto(inst, func, ...) ({			\
	__typeof__(inst) __inst = (inst);			\
	efi_fn_call(__inst, func, __inst, ##__VA_ARGS__);	\
})

#define efi_bs_call(func, ...) \
    efi_fn_call(efi_table_attr(efi_system_table, boottime), func, ##__VA_ARGS__)

#endif /* _DRIVERS_FIRMWARE_EFI_EFISTUB_H */
