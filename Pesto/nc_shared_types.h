#ifndef NC_SHARED_TYPES_H
#define NC_SHARED_TYPES_H

#include "nc_platform.h"
#include "nc_error.h"

#ifdef WINDOWS
	#include <stdint.h>
#else
	#include <stdlib.h>
	#include "pstdint.h"
#endif

#include <fitsio.h>
#include <tiffio.h>

#define OPENGL_ENABLE 0

#define NC_AUTO_DETECT 0x0000ffff
#define NC_AUTO_CHANNEL NC_AUTO_DETECT
#define NC_AUTO_UNIT	0x6fffffff
#define NC_FULL_WIDTH -1
#define NC_FULL_HEIGHT -1
#define NC_USE_MAC_ADRESS 0x20000000


// Type of amplifier set by the seleted readout mode
enum Ampli { NOTHING = 0, EM, CONV };

// Type of communication interface that can be used
enum CommType { 
	INVALID =		0x0000fffe,
	AUTO_COMM =		0x6fff0000, //NC_AUTO_UNIT - NC_AUTO_DETECT
	VIRTUAL =		0x7fff0000,
	EDT_CL =		0x00000000,
	PT_GIGE =		0x00010000,
	MIL_CL =		0x00020000,
	SAPERA_CL =		0x00030000,
	UNKNOWN =		0x00040000,
	REMOTE_COMM =	0x00050000
};

enum DetectorType { 
	CCD60 = 0, 
	CCD97, 
	CCD201_20, 
	CCD207_00, 
	CCD207_10, 
	CCD207_40, 
	CCD220, 
	UNKNOWN_CCD };

//All camera parameters
enum Features {
	UNKNOWN_FEATURE,
	EXPOSURE,
	WAITING_TIME,
	TRIGGER_MODE,
	SHUTTER_MODE,
	SHUTTER_POLARITY,
	EXTERNAL_SHUTTER,
	EXTERNAL_SHUTTER_MODE,
	EXTERNAL_SHUTTER_DELAY,
	FIRE_POLARITY,
	MINIMUM_PULSE_WIDTH,
	ARM_POLARITY,
	CALIBRATED_EM_GAIN,
	RAW_EM_GAIN,
	ANALOG_GAIN,
	ANALOG_OFFSET,
	TARGET_DETECTOR_TEMP,
	DETECTOR_TEMP,
	CTRL_TIMESTAMP,
	GPS_PRESENT,
	GPS_LOCKED,
	BINNING_X,
	BINNING_Y,
	ROI,
	CROP_MODE_X,
	CROP_MODE_Y,
	ACTIVE_REGION,
	MULTIPLE_ROI,
	TRIGGER_MODE_INTERNAL,	// INTERNAL trigger mode
	TRIGGER_MODE_EXTERNAL,	// EXT_HIGH_LOW or EXT_LOW_HIGH
	TRIGGER_MODE_EXPOSURE,	// EXT_HIGH_LOW_EXP or EXT_HIGH_LOW_EXP
	TRIGGER_MODE_CONTINUOUS,// CONT_HIGH_LOW or CONT_LOW_HIGH
	SYSTEM_STATUS,
	SHORT_SSVA_CMD,
	BIN_CDS
};

// For backwards compatibiliy
typedef enum Features Param;

// List of different temperatures
enum NcTemperatureType {
	NC_TEMP_CCD,
	NC_TEMP_CONTROLLER,
	NC_TEMP_POWER_SUPPLY,
	NC_TEMP_FPGA,
	NC_TEMP_HEATINK
};

// Reasons why a tap was reported as unused
enum NcPortUnusedReason {
	NC_PORT_UNUSED_NO_DEVICE,				// The port was found to not be connected to a device
	NC_PORT_UNUSED_ALREADY_IN_USE,			// The device is already in use by another instance
	NC_PORT_UNUSED_FILTERED,				// Port was not scanned (filtered, internal use only)
	NC_PORT_UNUSED_UNKNOWN,					// The reason was unusual and has no unique identifier

};

// Type of shutter mode available
enum ShutterMode { SHUT_NOT_SET = 0, OPEN = 1, CLOSE = 2, AUTO = 3, BIAS_DEFAULT = CLOSE };

// Type of trigger mode available
enum TriggerMode { 
	CONT_HIGH_LOW = -3,
	EXT_HIGH_LOW_EXP = -2,
	EXT_HIGH_LOW = -1,
	INTERNAL = 0,
	EXT_LOW_HIGH = 1,
	EXT_LOW_HIGH_EXP = 2,
	CONT_LOW_HIGH = 3};

// Type of crop-mode available
enum CropMode { 
	CROP_MODE_DISABLE = 0, 
	CROP_MODE_ENABLE_X = 1, 
	CROP_MODE_ENABLE_Y = 2, 
	CROP_MODE_ENABLE_XY = 3,
	CROP_MODE_ENABLE_ZL = CROP_MODE_ENABLE_X,
	CROP_MODE_ENABLE_SP = CROP_MODE_ENABLE_Y,
	CROP_MODE_ENABLE_MX = CROP_MODE_ENABLE_XY
	};

enum ExtShutter { EXTERNAL_SHUTTER_DISABLE = 0, EXTERNAL_SHUTTER_ENABLE = 1 };

enum ExtPolarity { NEGATIVE = -1, POL_NOT_SET = 0, POSITIVE = 1 };

// Type of image format which will be used to save an image
enum ImageFormat { UNKNOWNN = -1, TIF = 0, FITS };

// Use only CFITSIO DATATYPE codes
enum ImageDataType { NC_IMG,		// uint 16 bit
					 NC_IMG_UINT,	// uint 32 bit
					 NC_IMG_ULONG,	// uint 64 bit
					 NC_IMG_FLOAT}; // 32 bit float

enum HeaderDataType { NC_INT, NC_DOUBLE, NC_STRING };

enum ImageCompression { NO_COMPRESSION = 0, GZIP };

enum ProcType { NO_PROC = 0x00, LM = 0x01, PC = 0x02 };

enum TimestampMode { NO_TIMESTAMP = 0, INTERNAL_TIMESTAMP = 1, GPS_TIMESTAMP = 2 };

// List of possible version numbers
enum VersionType { SOFTWARE, FIRMWARE, FPGA, HARDWARE };

//Tif Tags
#define  AMPLI_TYPE_TAG							650
#define  HORIZ_FREQ_TAG							651
#define  VERT_FREQ_TAG							652
#define  EXPOSURE_TIME_TAG						653
#define  EFFECTIVE_EXP_TIME_TAG					682
#define  WAITING_TIME_TAG						654
#define  RAW_EM_GAIN_TAG						655
#define  CAL_EM_GAIN_TAG						680
#define  ANALOG_GAIN_TAG						656
#define  ANALOG_OFFSET_TAG						657
#define  TARGET_DETECTOR_TEMP_TAG				658
#define  BINNING_X_TAG							659
#define  BINNING_Y_TAG							660
#define  ROI_X1_TAG								661
#define  ROI_X2_TAG								662
#define  ROI_Y1_TAG								663
#define  ROI_Y2_TAG								664
#define  CROP_MODE_ENABLE_TAG					695
#define  CROP_MODE_PADDING_X_TAG				696
#define  CROP_MODE_PADDING_Y_TAG				697
#define  SHUTTER_MODE_TAG						665
#define  TRIGGER_MODE_TAG						666
#define  CLAMP_LEVEL_TAG						667
#define  PROC_TYPE_TAG							676
#define  NBR_PC_IMAGES_TAG						675
#define  SERIAL_NUMBER_TAG						668
#define  FIRM_VERSION_TAG						669
#define  HARDW_VERSION_TAG						670
#define  FPGA_VERSION_TAG						671
#define  SOFT_VERSION_TAG						672
#define  ADD_COMMENTS_TAG						674
#define  OVERSCAN_LINES_TAG						677
#define  OVERSAMPLE_X_TAG						678
#define  DETECTOR_TYPE_TAG						681
#define  FIRE_POLARITY_TAG						688
#define  SHUTTER_POLARITY_TAG					683
#define  ARM_POLARITY_TAG						689
#define  OUTPUT_PULSE_WIDTH_TAG					687
#define  EXT_SHUTTER_PRES_TAG					684
#define  EXT_SHUTTER_MODE_TAG					685
#define  EXT_SHUTTER_DELAY_TAG					686
#define  OBS_DATE_TAG							690
#define  OBS_TIME_SEC_FRACT_TAG					691
#define  OBS_FLAGS_TAG							692
#define  DATE_TIME_US_TAG						694
#define  HOST_TIME_TAG							693
#define  NBR_IMAGES_TRIG_TAG					679
#define  SEQUENCE_NAME_TAG						673


typedef unsigned short NcImage;
typedef void(*NcCallbackFunc)(void*);

typedef struct _NcStatsCtx NcStatsCtx;
typedef fitsfile NcCtxSaved;

typedef struct _NcDevice* NcDevice;


#ifdef WINDOWS
	#include <tchar.h>
	#include <conio.h>		
	#include <process.h>

	#ifdef NC_DRIVER_EXPORTS  	
		#define NC_FUNCTION __declspec(dllexport)
	#else
		#define NC_FUNCTION __declspec(dllimport)
	#endif
#else
	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <tiffio.h>
	#include <string.h>
	#define NC_FUNCTION
#endif

// Common data type enumeration
enum NcSdkDataTypes 
{
	NcSdkDataTypeInt = 0,
	NcSdkDataTypeDouble,
	NcSdkDataTypeString,
};	// enum NcSdkDataTypes

#endif	// NC_SHARED_TYPES_H
