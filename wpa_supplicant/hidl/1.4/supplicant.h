/*
 * hidl interface for wpa_supplicant daemon
 * Copyright (c) 2004-2016, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2016, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_SUPPLICANT_HIDL_SUPPLICANT_H
#define WPA_SUPPLICANT_HIDL_SUPPLICANT_H

#include <android/hardware/wifi/supplicant/1.0/ISupplicantCallback.h>
#include <android/hardware/wifi/supplicant/1.0/ISupplicantIface.h>
#include <android/hardware/wifi/supplicant/1.0/types.h>
#include <android/hardware/wifi/supplicant/1.4/ISupplicant.h>
#include <android-base/macros.h>
#include <hidl/Status.h>

extern "C"
{
#include "utils/common.h"
#include "utils/includes.h"
#include "utils/wpa_debug.h"
#include "wpa_supplicant_i.h"
#include "scan.h"
}

namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
namespace V1_4 {
namespace implementation {
using V1_0::ISupplicantCallback;
using V1_0::ISupplicantIface;

/**
 * Implementation of the supplicant hidl object. This hidl
 * object is used core for global control operations on
 * wpa_supplicant.
 */
class Supplicant : public V1_4::ISupplicant
{
public:
	Supplicant(struct wpa_global* global);
	~Supplicant() override = default;
	bool isValid();

	// Hidl methods exposed.
	Return<void> addInterface(
	    const IfaceInfo& iface_info, addInterface_cb _hidl_cb) override;
	Return<void> removeInterface(
	    const IfaceInfo& iface_info, removeInterface_cb _hidl_cb) override;
	Return<void> getInterface(
	    const IfaceInfo& iface_info, getInterface_cb _hidl_cb) override;
	Return<void> listInterfaces(listInterfaces_cb _hidl_cb) override;
	Return<void> registerCallback(
	    const sp<ISupplicantCallback>& callback,
	    registerCallback_cb _hidl_cb) override;
	Return<void> setDebugParams(
	    ISupplicant::DebugLevel level, bool show_timestamp, bool show_keys,
	    setDebugParams_cb _hidl_cb) override;
	Return<ISupplicant::DebugLevel> getDebugLevel() override;
	Return<bool> isDebugShowTimestampEnabled() override;
	Return<bool> isDebugShowKeysEnabled() override;
	Return<void> setConcurrencyPriority(
	    IfaceType type, setConcurrencyPriority_cb _hidl_cb) override;
	Return<void> terminate() override;

private:
	// Corresponding worker functions for the HIDL methods.
	std::pair<SupplicantStatus, sp<ISupplicantIface>> getInterfaceInternal(
	    const IfaceInfo& iface_info);
	std::pair<SupplicantStatus, sp<ISupplicantIface>> addInterfaceInternal(
	    const IfaceInfo& iface_info);
	SupplicantStatus removeInterfaceInternal(const IfaceInfo& iface_info);
	std::pair<SupplicantStatus, std::vector<ISupplicant::IfaceInfo>>
	listInterfacesInternal();
	SupplicantStatus registerCallbackInternal(
	    const sp<ISupplicantCallback>& callback);
	SupplicantStatus setDebugParamsInternal(
	    ISupplicant::DebugLevel level, bool show_timestamp, bool show_keys);
	SupplicantStatus setConcurrencyPriorityInternal(IfaceType type);
	std::pair<SupplicantStatus, sp<ISupplicantIface>> addP2pDevInterface(
	    struct wpa_interface iface_params);

	// Raw pointer to the global structure maintained by the core.
	struct wpa_global* wpa_global_;
	// Driver name to be used for creating interfaces.
	static const char kDriverName[];
	// wpa_supplicant.conf file location on the device.
	static const char kConfigFilePath[];

	DISALLOW_COPY_AND_ASSIGN(Supplicant);
};

}  // namespace implementation
}  // namespace V1_4
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // WPA_SUPPLICANT_HIDL_SUPPLICANT_H
