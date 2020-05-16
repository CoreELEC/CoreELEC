/*
 * Copyright (C) 2012 Jolla Ltd.
 * Author: Philippe De Swert <philippe.deswert@jollamobile.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <android-config.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <getopt.h>
#include <string.h>

#include <hardware/gps.h>

const GpsInterface* Gps = NULL;
const AGpsInterface* AGps = NULL;
const AGpsRilInterface* AGpsRil = NULL;
const GpsNiInterface *GpsNi = NULL;
const GpsXtraInterface *GpsExtra = NULL;
#ifdef HAVE_ULP
const UlpNetworkInterface *UlpNetwork = NULL;
const UlpPhoneContextInterface *UlpPhoneContext = NULL;
#endif /* HAVE_ULP */
char *apn = 0;
char *agps_server = 0;

static const GpsInterface* get_gps_interface()
{
  int error;
  hw_module_t* module;
  const GpsInterface* interface = NULL;
  struct gps_device_t *device;

  error = hw_get_module(GPS_HARDWARE_MODULE_ID, (hw_module_t const**)&module);

  if (!error)
  {
     error = module->methods->open(module, GPS_HARDWARE_MODULE_ID, (struct hw_device_t **) &device);
     fprintf(stdout, "*** device info\n id = %s\n name = %s\n author = %s\n",
     module->id, module->name, module->author);

     if (!error)
     {
       interface = device->get_gps_interface(device);
     }
  }
  else
  {
	fprintf(stdout, "*** GPS interface not found :\(\n Bye! \n");
	exit(1);
  }

  return interface;
}

static const AGpsInterface* get_agps_interface(const GpsInterface *gps)
{
  const AGpsInterface* interface = NULL;

  if (gps)
  {
    interface = (const AGpsInterface*)gps->get_extension(AGPS_INTERFACE);
  }
  return interface;
}

static const AGpsRilInterface* get_agps_ril_interface(const GpsInterface *gps)
{
  const AGpsRilInterface* interface = NULL;

  if (gps)
  {
    interface = (const AGpsRilInterface*)gps->get_extension(AGPS_RIL_INTERFACE);
  }
  return interface;
}

static const GpsNiInterface* get_gps_ni_interface(const GpsInterface *gps)
{
  const GpsNiInterface* interface = NULL;

  if(gps)
  {
    interface = (const GpsNiInterface*)gps->get_extension(GPS_NI_INTERFACE);
  }
  return interface;
}

static const GpsXtraInterface* get_gps_extra_interface(const GpsInterface *gps)
{
  const GpsXtraInterface* interface = NULL;

  if(gps)
  {
    interface = (const GpsXtraInterface*)gps->get_extension(GPS_XTRA_INTERFACE);
  }
  return interface;
}

#ifdef HAVE_ULP
static const UlpNetworkInterface* get_ulp_network_interface(const GpsInterface *gps)
{
  const UlpNetworkInterface* interface = NULL;

  if (gps)
  {
    interface = (const UlpNetworkInterface*)gps->get_extension(ULP_NETWORK_INTERFACE);
  }
  return interface;
}

static const UlpPhoneContextInterface* get_ulp_phone_context_interface(const GpsInterface *gps)
{
  const UlpPhoneContextInterface* interface = NULL;

  if (gps)
  {
    interface = (const UlpPhoneContextInterface*)gps->get_extension(ULP_PHONE_CONTEXT_INTERFACE);
  }
  return interface;
}
#endif /* HAVE_ULP */

static void location_callback(GpsLocation* location)
{
  fprintf(stdout, "*** location callback\n");
  fprintf(stdout, "flags:\t%d\n", location->flags);
  fprintf(stdout, "latitude: \t%lf\n", location->latitude);
  fprintf(stdout, "longtide: \t%lf\n", location->longitude);
  fprintf(stdout, "accuracy:\t%f\n", location->accuracy);
  fprintf(stdout, "utc: \t%ld\n", (long)location->timestamp);
}

static void status_callback(GpsStatus* status)
{
  fprintf(stdout, "*** status callback\n");

  switch (status->status)
  {
    case GPS_STATUS_NONE:
	fprintf(stdout, "*** no gps\n");
	break;
    case GPS_STATUS_SESSION_BEGIN:
	fprintf(stdout, "*** session begin\n");
	break;
    case GPS_STATUS_SESSION_END:
	fprintf(stdout, "*** session end\n");
	break;
    case GPS_STATUS_ENGINE_ON:
	fprintf(stdout, "*** engine on\n");
	break;
    case GPS_STATUS_ENGINE_OFF:
	fprintf(stdout, "*** engine off\n");
	break;
    default:
	fprintf(stdout, "*** unknown status\n");
  }
}

static void sv_status_callback(GpsSvStatus* sv_info)
{
  int i = 0;

  fprintf(stdout, "*** sv status\n");
  fprintf(stdout, "sv_size:\t%zu\n", sv_info->size);
  fprintf(stdout, "num_svs:\t%d\n", sv_info->num_svs);
  for(i=0; i < sv_info->num_svs; i++)
  {
        fprintf(stdout, "\t azimuth:\t%f\n", sv_info->sv_list[i].azimuth);
        fprintf(stdout, "\t elevation:\t%f\n", sv_info->sv_list[i].elevation);
	/* if prn > 65 and <= 88 this is a glonass sattelite */
        fprintf(stdout, "\t prn:\t%d\n", sv_info->sv_list[i].prn);
        fprintf(stdout, "\t size:\t%zu\n", sv_info->sv_list[i].size);
        fprintf(stdout, "\t snr:\t%f\n", sv_info->sv_list[i].snr);
  }

}

static void nmea_callback(GpsUtcTime timestamp, const char* nmea, int length)
{
	char buf[83];
  fprintf(stdout, "*** nmea info\n");
  fprintf(stdout, "timestamp:\t%ld\n", (long)timestamp);
  /* NMEA sentences can only be between 11 ($TTFFF*CC\r\n) and 82 characters long */
  if (length > 10 && length < 83) {
    strncpy(buf, nmea, length);
    buf[length] = '\0';
    fprintf(stdout, "nmea (%d): \t%s\n", length, buf);
  } else {
    fprintf(stdout, "Invalid nmea data\n");
  }
}

static void set_capabilities_callback(uint32_t capabilities)
{
  fprintf(stdout, "*** set capabilities\n");
  fprintf(stdout, "capability is %.8x\n", capabilities);
  /* do nothing */
}

static void acquire_wakelock_callback()
{
  fprintf(stdout, "*** acquire wakelock\n");
  /* do nothing */
}

static void release_wakelock_callback()
{
  fprintf(stdout, "*** release wakelock\n");
  /* do nothing */
}

struct ThreadWrapperContext {
    void (*func)(void *);
    void *user_data;
};

static void *thread_wrapper_context_main_func(void *user_data)
{
  struct ThreadWrapperContext *ctx = (struct ThreadWrapperContext *)user_data;

  fprintf(stderr, " **** Thread wrapper start (start=%p, arg=%p) ****\n",
          ctx->func, ctx->user_data);
  ctx->func(ctx->user_data);
  fprintf(stderr, " **** Thread wrapper end (start=%p, arg=%p) ****\n",
          ctx->func, ctx->user_data);

  free(ctx);

  return NULL;
}

static pthread_t create_thread_callback(const char* name, void (*start)(void *), void* arg)
{
  pthread_t thread_id;
  int error = 0;

  /* Wrap thread function, so we can return void * to pthread and log start/end of thread */
  struct ThreadWrapperContext *ctx = calloc(1, sizeof(struct ThreadWrapperContext));
  ctx->func = start;
  ctx->user_data = arg;

  fprintf(stderr, " ** Creating thread: '%s' (start=%p, arg=%p)\n", name, start, arg);
  /* Do not use a pthread_attr_t (we'd have to take care of bionic/glibc differences) */
  error = pthread_create(&thread_id, NULL, thread_wrapper_context_main_func, ctx);
  fprintf(stderr, " ** After thread_create: '%s', error=%d (start=%p, arg=%p)\n", name, error, start, arg);

  if(error != 0)
	return 0;

  return thread_id;
}

static void agps_handle_status_callback(AGpsStatus *status)
{
  if(status->type)
  {
        fprintf(stdout, "*** gps type %d\n", status->type);
  }
  switch (status->status)
  {
    case GPS_REQUEST_AGPS_DATA_CONN:
        fprintf(stdout, "*** data_conn_open\n");
#if ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR < 2
        AGps->data_conn_open(AGPS_TYPE_SUPL, apn, AGPS_APN_BEARER_IPV4);
#else
        AGps->data_conn_open(apn);
#endif
        break;
    case GPS_RELEASE_AGPS_DATA_CONN:
        fprintf(stdout, "*** data_conn_closed\n");
#if ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR < 2
        AGps->data_conn_closed(AGPS_TYPE_SUPL);
#else
        AGps->data_conn_closed();
#endif
        break;
    case GPS_AGPS_DATA_CONNECTED:
        fprintf(stdout, "*** data_conn_established\n");
        break;
    case GPS_AGPS_DATA_CONN_DONE:
        fprintf(stdout, "*** data_conn_done\n");
        break;
    case GPS_AGPS_DATA_CONN_FAILED:
        fprintf(stdout, "*** data_conn_FAILED\n");
        break;
  }
}

static void agps_ril_set_id_callback(uint32_t flags)
{
  fprintf(stdout, "*** set_id_cb\n");
  AGpsRil->set_set_id(AGPS_SETID_TYPE_IMSI, "000000000000000");
}

static void agps_ril_refloc_callback(uint32_t flags)
{
  fprintf(stdout, "*** refloc_cb\n");
  /* TODO : find out how to fill in location
  AGpsRefLocation location;
  AGpsRil->set_ref_location(&location, sizeof(location));
  */
}

static void ni_notify_callback (GpsNiNotification *notification)
{
  fprintf(stdout, "*** ni notification callback\n");
}

static void download_xtra_request_callback (void)
{
  fprintf(stdout, "*** xtra download request to client\n");
}

#ifdef HAVE_ULP
static void ulp_network_location_request_callback(UlpNetworkRequestPos *req)
{
  fprintf(stdout, "*** ulp network location request (request_type=%#x, interval_ms=%d, desired_position_source=%#x)\n",
          req->request_type, req->interval_ms, req->desired_position_source);
}

static void ulp_request_phone_context_callback(UlpPhoneContextRequest *req)
{
  fprintf(stdout, "*** ulp phone context request (context_type=%#x, request_type=%#x, interval_ms=%d)\n",
          req->context_type, req->request_type, req->interval_ms);

  if (UlpPhoneContext) {
      fprintf(stdout, "*** sending ulp phone context reply\n");
      UlpPhoneContextSettings settings;
      settings.context_type = req->context_type;
      settings.is_gps_enabled = 1;
      settings.is_agps_enabled = 1;
      settings.is_network_position_available = 1;
      settings.is_wifi_setting_enabled = 1;
      settings.is_battery_charging = 0;
      settings.is_enh_location_services_enabled = 1;
      UlpPhoneContext->ulp_phone_context_settings_update(&settings);
  }
}
#endif /* HAVE_ULP */

GpsCallbacks callbacks = {
  sizeof(GpsCallbacks),
  location_callback,
  status_callback,
  sv_status_callback,
  nmea_callback,
  set_capabilities_callback,
  acquire_wakelock_callback,
  release_wakelock_callback,
  create_thread_callback,
};

AGpsCallbacks callbacks2 = {
  agps_handle_status_callback,
  create_thread_callback,
};

AGpsRilCallbacks callbacks3 = {
  agps_ril_set_id_callback,
  agps_ril_refloc_callback,
  create_thread_callback,
};

GpsNiCallbacks callbacks4 = {
  ni_notify_callback,
  create_thread_callback,
};

GpsXtraCallbacks callbacks5 = {
  download_xtra_request_callback,
  create_thread_callback,
};

#ifdef HAVE_ULP
UlpNetworkLocationCallbacks callbacks6 = {
  ulp_network_location_request_callback,
};

UlpPhoneContextCallbacks callbacks7 = {
  ulp_request_phone_context_callback,
};
#endif /* HAVE_ULP */

void sigint_handler(int signum)
{
  fprintf(stdout, "*** cleanup\n");
  if(AGps)
  {
#if ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR < 2
        AGps->data_conn_closed(AGPS_TYPE_SUPL);
#else
        AGps->data_conn_closed();
#endif
  }
  if (Gps)
  {
	Gps->stop();
	Gps->cleanup();
  }
  exit (0);
}

int main(int argc, char *argv[])
{
  int sleeptime = 6000, opt, initok = 0;
  int coldstart = 0, extra = 0;
#ifdef HAVE_ULP
  int ulp = 0;
#endif
  struct timeval tv;
  int agps = 0, agpsril = 0, injecttime = 0, injectlocation = 0;
  char *location = 0, *longitude, *latitude;
  float accuracy = 100; /* Use 100m as location accuracy by default */

  while ((opt = getopt(argc, argv, "acl:p:s:rtux")) != -1)
  {
	switch (opt) {
		case 'a':
		   agps = 1;
		   fprintf(stdout, "*** Using agps\n");
                   break;
		case 'c':
		   coldstart = 1;
		   fprintf(stdout, "*** Using cold start\n");
		   break;
		case 'l':
		   injectlocation = 1;
		   location = optarg;
		   fprintf(stdout, "*** Location info %s will be injected\n", location);
                   break;
		case 'r':
		   agpsril = 1;
		   fprintf(stdout, "*** Using agpsril\n");
                   break;
		case 't':
		   injecttime = 1;
		   fprintf(stdout, "*** Timing info will be injected\n");
                   break;
		case 'p':
		   apn = optarg;
		   break;
		case 's':
		   agps_server = optarg;
		   break;
		case 'u':
#ifdef HAVE_ULP
		   ulp = 1;
#endif
		   break;
		case 'x':
                   extra = 1;
                   fprintf(stdout, "*** Allowing for Xtra downloads\n");
                   break;
		default:
                   fprintf(stderr, "\n Usage: %s \n \
			   \t-a for agps,\n \
			   \t-c for coldstarting the gps,\n \
			   \t-p <apn name> to specify an apn name,\n \
			   \t-s <agps_server:port> to specify a different supls server.\n \
		           \t-r for agpsril,\n \
			   \t-t to inject time,\n \
			   \t-u to use ULP (if available,\n \
                           \t-x deal with Xtra gps data.\n \
			   \tnone for standalone gps\n",
                           argv[0]);
                   exit(1);
               }
  }

  if(!apn)
  {
        apn = strdup("Internet");
  }

  fprintf(stdout, "*** setup signal handler\n");
  signal(SIGINT, sigint_handler);

  fprintf(stdout, "*** get gps interface\n");
  Gps = get_gps_interface();

  fprintf(stdout, "*** init gps interface\n");
  initok = Gps->init(&callbacks);
  fprintf(stdout, "*** setting positioning mode\n");
  /* need to be done before starting gps or no info will come out */
  if((agps||agpsril) && !initok)
	Gps->set_position_mode(GPS_POSITION_MODE_MS_BASED, GPS_POSITION_RECURRENCE_PERIODIC, 1000, 0, 0);
  else
	Gps->set_position_mode(GPS_POSITION_MODE_STANDALONE, GPS_POSITION_RECURRENCE_PERIODIC, 1000, 0, 0);

  if (Gps && !initok && (agps||agpsril))
  {
    fprintf(stdout, "*** get agps interface\n");
    AGps = get_agps_interface(Gps);
    if (AGps)
    {
	fprintf(stdout, "*** set up agps interface\n");
	AGps->init(&callbacks2);
	fprintf(stdout, "*** set up agps server\n");
	if(agps_server)
	{
		char *server,*port = 0;

		server =  strdup(agps_server);
		strtok_r (server, ":", &port);

		fprintf(stdout, "SUPL server: %s at port number: %s\n", server, port);
		AGps->set_server(AGPS_TYPE_SUPL, server, atoi(port));
		free(server);
	}
	else
		AGps->set_server(AGPS_TYPE_SUPL, "supl.google.com", 7276);
    }

    if(agpsril)
    {
	fprintf(stdout, "*** get agps ril interface\n");

	AGpsRil = get_agps_ril_interface(Gps);
	if (AGpsRil)
	{
		AGpsRil->init(&callbacks3);
	}
    }
    /* if coldstart is requested, delete all location info */
    if(coldstart)
    {
      fprintf(stdout, "*** delete aiding data\n");
      Gps->delete_aiding_data(GPS_DELETE_ALL);
    }
    
    if(extra)
    {
      fprintf(stdout, "*** xtra aiding data init\n");
      GpsExtra = get_gps_extra_interface(Gps);
      if(GpsExtra)
        GpsExtra->init(&callbacks5);
    }

    fprintf(stdout, "*** setting up network notification handling\n");
    GpsNi = get_gps_ni_interface(Gps);
    if(GpsNi)
    {
      GpsNi->init(&callbacks4);
    }

#ifdef HAVE_ULP
    if(ulp)
    {
        UlpNetwork = get_ulp_network_interface(Gps);
        if (UlpNetwork) {
        fprintf(stdout, "*** got ulp network interface\n");
            if (UlpNetwork->init(&callbacks6) != 0) {
                fprintf(stdout, "*** FAILED to init ulp network interface\n");
                UlpNetwork = NULL;
            }
        }
        else
            fprintf(stdout, "*** ULP failed!\n");

        UlpPhoneContext = get_ulp_phone_context_interface(Gps);
        if (UlpPhoneContext) {
            fprintf(stdout, "*** got ulp phone context interface\n");
            UlpPhoneContext->init(&callbacks7);
        }
    }
#endif /* HAVE_ULP */
  }
  if(injecttime)
  {
    fprintf(stdout, "*** aiding gps by injecting time information\n");
    gettimeofday(&tv, NULL);
    Gps->inject_time(tv.tv_sec, tv.tv_sec, 0);
  }

  if(injectlocation)
  {
    fprintf(stdout, "*** aiding gps by injecting location information\n");
    //Gps->inject_location(double latitude, double longitude, float accuracy);
    latitude = strtok(location, ",");
    longitude = strtok(NULL, ",");
    Gps->inject_location(strtod(latitude, NULL), strtod(longitude, NULL), accuracy);
  }

  fprintf(stdout, "*** start gps track\n");
  Gps->start();

  fprintf(stdout, "*** gps tracking started\n");

  while(sleeptime > 0)
  {
    fprintf(stdout, "*** tracking.... \n");
    sleep(100);
    sleeptime = sleeptime - 100;
  }

  if (AGps)
#if ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR < 2
        AGps->data_conn_closed(AGPS_TYPE_SUPL);
#else
        AGps->data_conn_closed();
#endif
  fprintf(stdout, "*** stop tracking\n");
  Gps->stop();
  fprintf(stdout, "*** cleaning up\n");
  Gps->cleanup();

  return 0;
}
