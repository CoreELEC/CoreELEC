/*
 * Copyright (C) 2013-2014 Canonical Ltd
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

#ifndef MEDIA_RECORDER_LAYER_H_
#define MEDIA_RECORDER_LAYER_H_

#include <stdint.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Media Recorder Observer API
    struct MediaRecorderObserver;
    struct MediaRecorderObserver *android_media_recorder_observer_new();

    typedef void (*media_recording_started_cb)(bool started, void *context);
    void android_media_recorder_observer_set_cb(struct MediaRecorderObserver *observer, media_recording_started_cb cb, void *context);

    struct MediaRecorderWrapper;
    struct CameraControl;

    // Values are from andoid /frameworks/av/include/media/mediarecorder.h
    typedef enum
    {
        ANDROID_VIDEO_SOURCE_DEFAULT = 0,
        ANDROID_VIDEO_SOURCE_CAMERA = 1,
        ANDROID_VIDEO_SOURCE_GRALLOC_BUFFER = 2
    } VideoSource;

    // Values are from andoid /system/core/include/system/audio.h
    typedef enum
    {
        ANDROID_AUDIO_SOURCE_DEFAULT             = 0,
        ANDROID_AUDIO_SOURCE_MIC                 = 1,
        ANDROID_AUDIO_SOURCE_VOICE_UPLINK        = 2,
        ANDROID_AUDIO_SOURCE_VOICE_DOWNLINK      = 3,
        ANDROID_AUDIO_SOURCE_VOICE_CALL          = 4,
        ANDROID_AUDIO_SOURCE_CAMCORDER           = 5,
        ANDROID_AUDIO_SOURCE_VOICE_RECOGNITION   = 6,
        ANDROID_AUDIO_SOURCE_VOICE_COMMUNICATION = 7,
        ANDROID_AUDIO_SOURCE_REMOTE_SUBMIX       = 8,
        ANDROID_AUDIO_SOURCE_CNT,
        ANDROID_AUDIO_SOURCE_MAX                 = ANDROID_AUDIO_SOURCE_CNT - 1
    } AudioSource;

    // Values are from andoid /frameworks/av/include/media/mediarecorder.h
    typedef enum
    {
        ANDROID_OUTPUT_FORMAT_DEFAULT = 0,
        ANDROID_OUTPUT_FORMAT_THREE_GPP = 1,
        ANDROID_OUTPUT_FORMAT_MPEG_4 = 2,
        ANDROID_OUTPUT_FORMAT_AUDIO_ONLY_START = 3,
        /* These are audio only file formats */
        ANDROID_OUTPUT_FORMAT_RAW_AMR = 3, // to be backward compatible
        ANDROID_OUTPUT_FORMAT_AMR_NB = 3,
        ANDROID_OUTPUT_FORMAT_AMR_WB = 4,
        ANDROID_OUTPUT_FORMAT_AAC_ADIF = 5,
        ANDROID_OUTPUT_FORMAT_AAC_ADTS = 6,
        /* Stream over a socket, limited to a single stream */
        ANDROID_OUTPUT_FORMAT_RTP_AVP = 7,
        /* H.264/AAC data encapsulated in MPEG2/TS */
        ANDROID_OUTPUT_FORMAT_MPEG2TS = 8
    } OutputFormat;

    // Values are from andoid /frameworks/av/include/media/mediarecorder.h
    typedef enum
    {
        ANDROID_VIDEO_ENCODER_DEFAULT = 0,
        ANDROID_VIDEO_ENCODER_H263 = 1,
        ANDROID_VIDEO_ENCODER_H264 = 2,
        ANDROID_VIDEO_ENCODER_MPEG_4_SP = 3
    } VideoEncoder;

    // Values are from andoid /frameworks/av/include/media/mediarecorder.h
    typedef enum
    {
        ANDROID_AUDIO_ENCODER_DEFAULT = 0,
        ANDROID_AUDIO_ENCODER_AMR_NB = 1,
        ANDROID_AUDIO_ENCODER_AMR_WB = 2,
        ANDROID_AUDIO_ENCODER_AAC = 3,
        ANDROID_AUDIO_ENCODER_HE_AAC = 4,
        ANDROID_AUDIO_ENCODER_AAC_ELD = 5
    } AudioEncoder;

    /* Defines how many bytes to read of the microphone at a time. This value
       is how many bytes AudioFlinger would read max at a time from the microphone,
       so duplicate using that value here since that code is well tested. */
    #define MIC_READ_BUF_SIZE 960

    // Callback types
    typedef void (*on_recorder_msg_error)(void *context);
    typedef void (*on_recorder_read_audio)(void *context);

    // Callback setters
    void android_recorder_set_error_cb(struct MediaRecorderWrapper *mr, on_recorder_msg_error cb,
                                       void *context);
    void android_recorder_set_audio_read_cb(struct MediaRecorderWrapper *mr, on_recorder_read_audio cb,
                                       void *context);

    // Main recorder control API
    struct MediaRecorderWrapper *android_media_new_recorder();
    int android_recorder_initCheck(struct MediaRecorderWrapper *mr);
    int android_recorder_setCamera(struct MediaRecorderWrapper *mr, struct CameraControl* control);
    int android_recorder_setVideoSource(struct MediaRecorderWrapper *mr, VideoSource vs);
    int android_recorder_setAudioSource(struct MediaRecorderWrapper *mr, AudioSource as);
    int android_recorder_setOutputFormat(struct MediaRecorderWrapper *mr, OutputFormat of);
    int android_recorder_setVideoEncoder(struct MediaRecorderWrapper *mr, VideoEncoder ve);
    int android_recorder_setAudioEncoder(struct MediaRecorderWrapper *mr, AudioEncoder ae);
    int android_recorder_setOutputFile(struct MediaRecorderWrapper *mr, int fd);
    int android_recorder_setVideoSize(struct MediaRecorderWrapper *mr, int width, int height);
    int android_recorder_setVideoFrameRate(struct MediaRecorderWrapper *mr, int frames_per_second);
    int android_recorder_setParameters(struct MediaRecorderWrapper *mr, const char* parameters);
    int android_recorder_start(struct MediaRecorderWrapper *mr);
    int android_recorder_stop(struct MediaRecorderWrapper *mr);
    int android_recorder_prepare(struct MediaRecorderWrapper *mr);
    int android_recorder_reset(struct MediaRecorderWrapper *mr);
    int android_recorder_close(struct MediaRecorderWrapper *mr);
    int android_recorder_release(struct MediaRecorderWrapper *mr);

#ifdef __cplusplus
}
#endif

#endif
