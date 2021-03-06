/*
 * Copyright (C) 2016 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ZERO_HARDWARE_POWER_V1_0_POWER_H
#define ZERO_HARDWARE_POWER_V1_0_POWER_H

#include <android/hardware/power/1.0/IPower.h>

#ifdef POWER_HAS_LINEAGE_HINTS
#include <vendor/lineage/power/1.0/ILineagePower.h>
#endif

#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define POWER_DT2W_ENABLED                "/sys/class/dt2w/enabled"
#define POWER_TOUCHKEYS_ENABLED           "/sys/class/sec/sec_touchkey/input/enabled"
#define POWER_TOUCHKEYS_BRIGHTNESS        "/sys/class/sec/sec_touchkey/brightness"
#define POWER_FINGERPRINT_PM			  "/sys/class/fingerprint/fingerprint/pm"
#define POWER_FINGERPRINT_WAKELOCKS       "/sys/class/fingerprint/fingerprint/wakelocks"

#define POWER_TOUCHSCREEN_NAME            "/sys/class/input/input1/name"
#define POWER_TOUCHSCREEN_NAME_EXPECT     "sec_touchscreen"
#define POWER_TOUCHSCREEN_ENABLED_FLAT    "/sys/class/input/input1/enabled"
#define POWER_TOUCHSCREEN_ENABLED_EDGE    "/sys/class/input/input0/enabled"

#define __CPU_ONLINE(core, state) \
{ \
	ostringstream cpu_state_path; \
	cpu_state_path << "/sys/devices/system/cpu/cpu" << core << "/online"; \
	Utils::write(cpu_state_path.str().c_str(), !!state); \
}

#define CPU_OFFLINE(core)  __CPU_ONLINE(core, 0)
#define CPU_ONLINE(core)  __CPU_ONLINE(core, 1)

#define __CPU_ONLINE_ALL(state) \
{ \
	for (int __coai = 0; __coai < NR_CPUS; __coai++) \
		__CPU_ONLINE(__coai, state) \
}

#define CPU_OFFLINE_ALL()  __CPU_ONLINE_ALL(0)
#define CPU_ONLINE_ALL()  __CPU_ONLINE_ALL(1)

/*
 * Write cpugov-independent settings
 */
// single-argument macro, var == filename
#define __cpu_cluster_write(core, cluster, var)  Utils::writeCpuGov(core, #var, data->cpu.cluster.var)
#define cpu_apollo_write(var)  __cpu_cluster_write(0, apollo, var)
#define cpu_atlas_write(var)  __cpu_cluster_write(4, atlas, var)

// double-argument macro, var != filename
#define __cpu_cluster_write2(core, cluster, var, file)  Utils::writeCpuGov(core, file, data->cpu.cluster.var)
#define cpu_apollo_write2(var, file)  __cpu_cluster_write2(0, apollo, var, file)
#define cpu_atlas_write2(var, file)  __cpu_cluster_write2(4, atlas, var, file)

/*
 * Write cpugov-specific settings
 */
// single-argument macro, var == filename
#define __cpugov_cluster_write(cpugov, core, cluster, var)  Utils::writeCpuGov(core, #var, data->cpu.cluster.cpugov.var)
#define cpugov_apollo_write(cpugov, var)  __cpugov_cluster_write(cpugov, 0, apollo, var)
#define cpugov_atlas_write(cpugov, var)  __cpugov_cluster_write(cpugov, 4, atlas, var)

// double-argument macro, var != filename
#define __cpugov_cluster_write2(cpugov, core, cluster, var, file)  Utils::writeCpuGov(core, file, data->cpu.cluster.cpugov.var)
#define cpugov_apollo_write2(cpugov, var, file)  __cpugov_cluster_write2(cpugov, 0, apollo, var, file)
#define cpugov_atlas_write2(cpugov, var, file)  __cpugov_cluster_write2(cpugov, 4, atlas, var, file)

/*
 * Assert cpugovs
 */
#define __if_cluster_cpugov(core, gov)  if (Utils::assertCpuGov(core, gov))
#define if_apollo_cpugov(gov)  __if_cluster_cpugov(0, gov)
#define if_atlas_cpugov(gov)  __if_cluster_cpugov(4, gov)

/*
 * Quick-Casts
 */
#define cint(data)  static_cast<int>(data)

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::IPower;
using ::android::hardware::power::V1_0::PowerHint;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;
#ifdef POWER_HAS_LINEAGE_HINTS
using ::vendor::lineage::power::V1_0::ILineagePower;
using ::vendor::lineage::power::V1_0::LineageFeature;
using ::vendor::lineage::power::V1_0::LineagePowerHint;
#endif

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;
using ::android::status_t;
	
using ::std::string;

#ifdef LOCK_PROTECTION
  #define power_lock() \
      std::lock_guard<std::mutex> autolock(mLock)
#else
	#define power_lock()
#endif

#define switch_uint32_t(_data)    switch (static_cast<uint32_t>(_data))
#define case_uint32_t(_feature)    case static_cast<uint32_t>(_feature)

enum class SecPowerProfiles : int32_t {
	INVALID          = (-0x7FFFFFFF - 1),
	SCREEN_OFF       = -1,
	POWER_SAVE       = 0,
	BALANCED         = 1,
	HIGH_PERFORMANCE = 2,
	BIAS_POWER_SAVE  = 3,
	BIAS_PERFORMANCE = 4,
	MAX_PROFILES     = 5,
	MAX              = 6
};

enum class SecDeviceVariant : int32_t {
	UNKNOWN = 0,
	FLAT = 1,
	EDGE = 2,
};

struct Power : 
      public IPower
#ifdef POWER_HAS_LINEAGE_HINTS
    , public ILineagePower
#endif
{

	Power();
	~Power();

	status_t registerAsSystemService();

	// Methods from ::android::hardware::power::V1_0::IPower follow.
	Return<void> setInteractive(bool interactive)  override;
	Return<void> powerHint(PowerHint hint, int32_t data)  override;
	Return<void> setFeature(Feature feature, bool activate)  override;
	Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb)  override;

	// Methods from ::vendor::lineage::power::V1_0::ILineagePower follow.
#ifdef POWER_HAS_LINEAGE_HINTS
	Return<int32_t> getFeature(LineageFeature feature)  override;
#endif

private:
#ifdef LOCK_PROTECTION
	mutable std::mutex mLock;
#endif
	
	// Stores the current interactive-state of the device
	// Default to <false>.
	bool mIsInteractive;

	// profiles requested by SET_PROFILE-hint.
	// Default to <INVALID>.
	SecPowerProfiles mRequestedProfile;

	// profile currently in use. may be set by every power-method.
	// Default to <INVALID>.
	SecPowerProfiles mCurrentProfile;

	// The device-variant this power-service is currently running on
	// Default to <UNKNOWN>.
	SecDeviceVariant mVariant;
	
	// The path to control the state of the touchscreen. May vary between
	// different variants.
	// Default to <"">.
	string mTouchControlPath;
	
	// Stores the current state of the touchkeys to prevent accidental
	// enabling if user decidec to use on-screen-navbar and disabled them
	// Default to <true>.
	bool mTouchkeysEnabled;

	// Stores the user-set doubletap2wake-state
	// Default to <false>.
	bool mIsDT2WEnabled;

	//
	// Private methods
	//

	// Set the current profile to [profile]. Also updates mCurrentProfile.
	// Default to UNKNOWN
	void setProfile(SecPowerProfiles profile);

	// Either resets the current profile to mRequestedProfile or
	// falls back to BALANCED if mRequestedProfile is set to INVALID.
	void resetProfile();

	// updates the current state of managed input-devices.
	void setInputState(bool enabled);

	// updates the current state the fingerprint-sensor
	void setFingerprintState(bool enabled);

	// updates the current state the doubletap2wake-capability. uses the 
	// global member [mIsDT2WEnabled] to determine the new state
	void setDT2WState();

};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // ZERO_HARDWARE_POWER_V1_0_POWER_H
