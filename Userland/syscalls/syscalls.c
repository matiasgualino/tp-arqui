#include <syscalls.h>
#include <types.h>
#include <stdint.h>

extern uint64_t syscall(uint64_t callid, ...);

void sys_write(FD fd, char* s, int len) {

	syscall((uint64_t)SYSCALL_WRITE, (uint64_t)fd, (uint64_t)s, (uint64_t)len);

}

int sys_read(FD fd, char* s, int len) {

	return syscall((uint64_t)SYSCALL_READ, (uint64_t)fd, (uint64_t)s, (uint64_t)len);

}

void sys_rtc_get(time_t* t) {
	syscall((uint64_t)SYSCALL_RTC, (uint64_t)t);
}

void sys_rtc_set(time_t* t) {
	syscall((uint64_t)SYSCALL_RTC_SET, (uint64_t)t);
}

void* sys_malloc(int len) {
	return (void*)syscall((uint64_t)SYSCALL_MALLOC, (uint64_t)len);
}

void* sys_calloc(int len) {
	return (void*)syscall((uint64_t)SYSCALL_CALLOC, (uint64_t)len);
}

void sys_free(void* m) {
	syscall((uint64_t)SYSCALL_FREE, (uint64_t)m);
}

void sys_keyboard_catch(uint64_t scancode, dka_handler handler) {
	syscall((uint64_t)SYSCALL_KEYBOARD_CATCH, (uint64_t)scancode, (uint64_t)handler);
}

void sys_clear_indexed_line(int index) {
	syscall((uint64_t)SYSCALL_VIDEO_CLR_INDEXED_LINE, (uint64_t)index);
}

void sys_keyboard_replace_buffer(char* s) {
	syscall((uint64_t)SYSCALL_KEYBOARD_REPLACE_BUFFER, (uint64_t)s);
}

color_t sys_get_color() {
	return (color_t)syscall((uint64_t)SYSCALL_GET_COLOR);
}

void sys_set_color(color_t t) {
	syscall((uint64_t)SYSCALL_SET_COLOR, (uint64_t)t);
}

void sys_kbd_set_distribution(keyboard_distrib d) {
	syscall((uint64_t)SYSCALL_SET_KBD_DISTRIBUTION, (uint64_t)d);
}

void sys_set_screensaver_timer(uint64_t t) {

	syscall((uint64_t)SYSCALL_SCREENSAVER_TIMER, t);
}

void sys_clear_screen(){
	syscall(SYSCALL_CLEAR_SCREEN);
}

void sys_screensaver_trigger() {
	syscall((uint64_t) SYSCALL_SCREENSAVER_TRIGGER);
}

void sys_exit(){
	syscall(SYSCALL_EXIT);
}

