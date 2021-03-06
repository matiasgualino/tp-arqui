#include <stdint.h>

#if ! MACOS
#include <string.h>
#endif

#include <lib.h>
#include <moduleLoader.h>
#include <video.h>
#include <keyboard.h>
#include <types.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern char keyboard_kbuffer[KEYBOARD_BUFFER_SIZE];

static const uint64_t PageSize = 0x1000;

static void * const shellCodeModuleAddress = (void*)0x400000;
static void * const shellDataModuleAddress = (void*)0x500000;

static uint64_t pit_timer = 0;
//Screensaver, 20 segundos por defecto
uint64_t screensaver_wait_time = 20;
uint64_t screensaver_timer = 0;
bool screensaver_is_active = FALSE;

typedef int (*EntryPoint)();

void load_kernel_modules();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
	           (uint64_t)&endOfKernel
	           + PageSize * 8				//The size of the stack itself, 32KiB
	           - sizeof(uint64_t)			//Begin at the top of the stack
	       );
}

void * initializeKernelBinary() {

	load_kernel_modules();

	clearBSS(&bss, &endOfKernel - &bss);


	video_initialize();
	video_clear_screen();

	video_write_line("[x64BareBones]");

	video_write_string("  text: 0x");
	video_write_hex((uint64_t)&text);
	video_write_nl();

	video_write_string("  rodata: 0x");
	video_write_hex((uint64_t)&rodata);
	video_write_nl();

	video_write_string("  data: 0x");
	video_write_hex((uint64_t)&data);
	video_write_nl();

	video_write_string("  bss: 0x");
	video_write_hex((uint64_t)&bss);
	video_write_nl();

	video_write_line("[Done]");

	screensaver_reset_timer();

	video_write_line("Kernel cargado.");

	return getStackBase();
}

void load_kernel_modules() {

	void * moduleAddresses[] = {
		shellCodeModuleAddress,
		shellDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

}

int main() {

	video_write_string("Keyboard buffer at: 0x");
	video_write_hex((uint64_t)&keyboard_kbuffer);
	video_write_nl();
	video_write_string("Keyboard buffer size: ");
	video_write_dec((uint64_t)KEYBOARD_BUFFER_SIZE);
	video_write_nl();

	video_write_line("[Kernel Main]");

	video_write_string("  Shell code module at 0x");
	video_write_hex((uint64_t)shellCodeModuleAddress);
	video_write_nl();

	video_write_line("Calling shell module...");
	video_write_nl();
	((EntryPoint)shellCodeModuleAddress)();

	return 0;
}

//retorna si se debe ignorar lo tecleado
bool screensaver_reset_timer() {

	bool ret = FALSE;

	if (screensaver_is_active) {
		ret = TRUE;
		screensaver_is_active = FALSE;
		video_trigger_restore();

	}
	screensaver_timer = 18 * screensaver_wait_time;

	return ret;

}

void irq0_handler() {

	pit_timer++;
	screensaver_timer--;

	if (screensaver_timer == 0 && !screensaver_is_active) {
		active_screensaver();
	}

}

void active_screensaver() {
	screensaver_is_active = TRUE;
	video_trigger_screensaver();
}
