/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "Ver2-ULP-Components"
 * 	found in "../ver2-ulp-components.asn"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_SET_GANSSReferenceTime_H_
#define	_SET_GANSSReferenceTime_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>
#include "PrimaryCPICH-Info.h"
#include "CellParametersID.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SET_GANSSReferenceTime__modeSpecificInfo_PR {
	SET_GANSSReferenceTime__modeSpecificInfo_PR_NOTHING,	/* No components present */
	SET_GANSSReferenceTime__modeSpecificInfo_PR_fdd,
	SET_GANSSReferenceTime__modeSpecificInfo_PR_tdd
} SET_GANSSReferenceTime__modeSpecificInfo_PR;

/* SET-GANSSReferenceTime */
typedef struct SET_GANSSReferenceTime {
	struct SET_GANSSReferenceTime__set_GANSSTimingOfCell {
		long	 ms_part;
		unsigned long	 ls_part;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *set_GANSSTimingOfCell;
	struct SET_GANSSReferenceTime__modeSpecificInfo {
		SET_GANSSReferenceTime__modeSpecificInfo_PR present;
		union SET_GANSSReferenceTime__modeSpecificInfo_u {
			struct SET_GANSSReferenceTime__modeSpecificInfo__fdd {
				PrimaryCPICH_Info_t	 referenceIdentity;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} fdd;
			struct SET_GANSSReferenceTime__modeSpecificInfo__tdd {
				CellParametersID_t	 referenceIdentity;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} tdd;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *modeSpecificInfo;
	long	 sfn;
	long	*ganss_TODUncertainty	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SET_GANSSReferenceTime_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ls_part_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SET_GANSSReferenceTime;

#ifdef __cplusplus
}
#endif

#endif	/* _SET_GANSSReferenceTime_H_ */
#include <asn_internal.h>
