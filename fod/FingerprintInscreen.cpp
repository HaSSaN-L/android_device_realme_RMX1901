/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "FingerprintInscreenService"

#include "FingerprintInscreen.h"
#include <hidl/HidlTransportSupport.h>
#include <android-base/logging.h>
#include <fstream>
#include <cmath>
#include <unistd.h>
#define FP_PRESS_PATH "/sys/kernel/oppo_display/notify_fppress"
#define HBM_PATH "/sys/kernel/oppo_display/hbm"
#define DIM_PATH "/sys/kernel/oppo_display/dimlayer_hbm"
#define DIM_AMOUNT_PATH "/sys/kernel/oppo_display/dim_alpha"

namespace {

template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
    LOG(INFO) << "wrote path: " << path << ", value: " << value << "\n";
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

} // anonymous namespace

namespace vendor {
namespace lineage {
namespace biometrics {
namespace fingerprint {
namespace inscreen {
namespace V1_0 {
namespace implementation {

FingerprintInscreen::FingerprintInscreen() {
}

Return<int32_t> FingerprintInscreen::getPositionX() {
    return 442;
}

Return<int32_t> FingerprintInscreen::getPositionY() {
    return 1986;
}

Return<int32_t> FingerprintInscreen::getSize() {
    return 196;
}

Return<void> FingerprintInscreen::onStartEnroll() {
     set(HBM_PATH, 1);
     set(DIM_PATH, 1);
return Void();
}

Return<void> FingerprintInscreen::onFinishEnroll() {
    set(HBM_PATH, 0);
    set(DIM_PATH, 0);
return Void();
}

Return<void> FingerprintInscreen::onPress() {
//    set(HBM_PATH, 1);
    set(FP_PRESS_PATH, 1);
    return Void();
}

Return<void> FingerprintInscreen::onRelease() {
//    set(HBM_PATH, 0);
    set(FP_PRESS_PATH, 0);
    return Void();
}

Return<void> FingerprintInscreen::onShowFODView() {
    set(HBM_PATH, 1);
    set(DIM_PATH, 1);
return Void();
}

Return<void> FingerprintInscreen::onHideFODView() {
    set(HBM_PATH, 0);
    set(FP_PRESS_PATH, 0);
    set(DIM_PATH, 0);
return Void();
}

Return<bool> FingerprintInscreen::handleAcquired(int32_t acquiredInfo, int32_t vendorCode) {
    LOG(ERROR) << "acquiredInfo: " << acquiredInfo << ", vendorCode: " << vendorCode << "\n";
    return false;
}

Return<bool> FingerprintInscreen::handleError(int32_t error, int32_t vendorCode) {
    LOG(ERROR) << "error: " << error << ", vendorCode: " << vendorCode << "\n";
    return false;
}

Return<void> FingerprintInscreen::setLongPressEnabled(bool) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getDimAmount(int32_t brightness) {	
/*
Return<int32_t> FingerprintInscreen::getDimAmount(int32_t) {
    int dimAmount = get(DIM_AMOUNT_PATH, 0);
    LOG(INFO) << "dimAmount = " << dimAmount;

    return dimAmount;
*/

float alpha;
    if (brightness > 62) {
        alpha = 1.0 - pow(brightness / 255.0 * 430.0 / 600.0, 0.45);
    } else {
        alpha = 1.0 - pow(brightness / 200.0, 0.45);
    }
    return 255 * alpha;
}

Return<bool> FingerprintInscreen::shouldBoostBrightness() {
    return false;
}

Return<void> FingerprintInscreen::setCallback(const sp<::vendor::lineage::biometrics::fingerprint::inscreen::V1_0::IFingerprintInscreenCallback>& callback) {
    {
        std::lock_guard<std::mutex> _lock(mCallbackLock);
        mCallback = callback;
    }
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace inscreen
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace lineage
}  // namespace vendor
