#define LOG_TAG "ccc ITeeveePlayerService"

#include <cutils/log.h>

#include "../TestBinderServer/ITestBinderService.h"

namespace android {

enum {
	TEST_ADD = IBinder::FIRST_CALL_TRANSACTION,
};

class BpTestBinderService: public BpInterface<ITestBinderService> {
public:
	BpTestBinderService(const sp<IBinder>& impl) :
		BpInterface<ITestBinderService> (impl) {
	}

	int add(int a, int b) {
		
		Parcel data, reply;
		ALOGI("Enter BpTestBinderService add,a = %d , b = %d", a, b);
		data.writeInterfaceToken(ITestBinderService::getInterfaceDescriptor());
		data.writeInt32(a);
		data.writeInt32(b);
		remote()->transact(TEST_ADD, data, &reply);
		int sum = reply.readInt32();
		ALOGI("BpTestBinderService sum = %d", sum);
		return sum;
	}
};

IMPLEMENT_META_INTERFACE(TestBinderService, "android.test.ITestBinderService");

// ----------------------------------------------------------------------

status_t BnTestBinderService::onTransact(uint32_t code, const Parcel& data,
		Parcel* reply, uint32_t flags) {
	switch (code) {
	case TEST_ADD: {
		
		CHECK_INTERFACE(ITestBinderService, data, reply);
		int a = data.readInt32();
		int b = data.readInt32();
		ALOGI("Enter BnTestBinderService add,a = %d , b = %d", a, b);
		int sum = 0;
		sum  = add(a, b);
		ALOGI("BnTestBinderService sum = %d", sum);
		 reply->writeInt32(sum);
		return sum;
	}
	default:
		return BBinder::onTransact(code, data, reply, flags);
	}
}

}
