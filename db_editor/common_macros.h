#pragma once

#define DECLARE_NO_COPY_NO_MOVE(__dlg__)\
	__dlg__(const __dlg__&) = delete;\
	__dlg__(__dlg__&&) noexcept = delete;\
	__dlg__& operator=(const __dlg__&) = delete;\
	__dlg__& operator=(__dlg__&&) noexcept = delete;
