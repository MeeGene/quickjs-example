#pragma comment(lib, "libquickjs")

#include <string>

#include "quickjs-libc.h"

bool initQuickJS(const std::string& path) {
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);
    
    if (!ctx) {
        return false;
    }

    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
    js_std_add_helpers(ctx, 0, NULL);

    std::size_t length;
	char* buffer = reinterpret_cast<char*>(js_load_file(ctx, &length, path.c_str()));
	if (!buffer) {
		return false;
	}
    
    JSValue value = JS_Eval(ctx, buffer, length, path.c_str(), JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(value)) {
        js_std_dump_error(ctx);
        JS_FreeValue(ctx, value);
        return false;
    }
    JS_FreeValue(ctx, value);
    js_free(ctx, buffer);
    return true;
}

int main(int argc, char* argv[]) {
    if (!initQuickJS("test.js")) {
        return -1;
    }
    return 0;
}