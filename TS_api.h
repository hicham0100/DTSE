/********************************************************************************
 * Schneider-Electric                                                           *
 * Global Solutions - Digital Services Transformation                           *
 * Digital Services Platform                                                    *
 * Copyright (c) 2019 - All rights reserved.                                    *
 *                                                                              *
 * Developed by:                                                                *
 *          Hicham Hossayni                                                     *
 *                                                                              *
 * No part of this document may be reproduced in any form without the           *
 * express written consent of Schneider-Electric.                               *
 ********************************************************************************/

/**
 * @file
 *
 * @author Hicham Hossayni 
 */

#ifndef TSAPI_H_
#define TSAPI_H_

#include "timeSeries_Manager.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the Time Series storage module, must be called before calling any other time series function
 * @return @DTSE_SUCCESS on success or another error code.
 */
DTSE_STATUS   TS_init			();

/**
 * Releases all the resources allocated by the time series storage module
 * @return @DTSE_SUCCESS on success or another error code.
 */
DTSE_STATUS   TS_Close			();

/**
 * Creates a new time series entry
 * @param id 	a uniq time series ID
 * @param type  the type of the time series value
 * @return @DTSE_SUCCESS on success or another error code.
 */
DTSE_STATUS TS_NewTimeSeries(char * id, TS_valueType type);


/**
 * Insert a new time series value
 * @param id		the id of the time series
 * @param time		0 or the real value timestamp
 * @param value		The value to be inserted
 * @return @DTSE_SUCCESS on success or another error code.
 */
DTSE_STATUS   TS_Insert			(char * id, DTSE_time time, DTSE_double value);

/**
 * Selects the last @N elements of the time series filtered by a simple operation
 * @param id		Time series Id
 * @param N			Number of needed elements
 * @param op		Comparison operator
 * @param value		Operand for the comparison
 * @param status	Pointer to store the status of the operation
 * @return
 */
s_TS_Value * TS_Select			(char * id, DTSE_int N,DTSE_operator op, double value, DTSE_STATUS * status);

/**
 * Selects the time series elements between two timestamps filtered by a simple operation
 * @param id		Time series Id
 * @param from  	The lowest time stamp
 * @param to		The highest time stamp
 * @param op		Comparison operator
 * @param value 	Operand for the comparison
 * @param status	Pointer to store the status of the operation
 * @return
 */
s_TS_Value * TS_SelectBetween	(char * id, DTSE_time from, DTSE_time to,DTSE_operator op, double value,  DTSE_STATUS * status);

/**
 * Deletes the oldest N entries of a time series
 * @param id	Time series Id
 * @param N		Number of entries to be deleted
 * @return @DTSE_SUCCESS on success or another error code.
 */
DTSE_STATUS   TS_delete			(char * id, int N);

/**
 * Deletes all entries before a given timestamp
 * @param id	Time series Id
 * @param time	the timestamp
 * @return @DTSE_SUCCESS on success or another error code.
 */
DTSE_STATUS   TS_deleteBefore	(char * id, DTSE_time time);

/**
 * Search the entries of a time series based on several complex conditions
 * @param id				Time series Id
 * @param valueCond			Conditions on the value
 * @param timeCond			Conditions on the time composites (year, month, day, hour, minutes)
 * @param timeRanges		Time ranges conditions
 * @param status			Pointer to store the status of the operation.
 * @return	@s_TS_Value values or NULL for failure or zero results
 */

s_TS_Value * DTSE_TS_Select 	(char * id, s_TS_Condition * valueCond,s_TS_Condition * timeCond,
							 s_TS_TimeRange * timeRanges,DTSE_STATUS * status);

/**
 * Search the time ranges during which all the given conditions are satisfied
 * @param id				Time series Id
 * @param valueCond			Conditions on the value
 * @param timeCond			Conditions on the time composites (year, month, day, hour, minutes)
 * @param duration			Conditions on the duration of the timerange
 * @param timeRanges		Time ranges conditions
 * @param status			Pointer to store the status of the operation.
 * @return	@s_TS_TimeRange time ranges or NULL for failure or zero results
 */
s_TS_TimeRange * DTSE_TS_SelectTimes 	(char * id, s_TS_Condition * valueCond,s_TS_Condition * timeCond,
		s_TS_Condition * duration, s_TS_TimeRange * timeRanges, DTSE_STATUS * status);


/**
 * Search and Aggregate values of a time series based on several complex conditions
 * @param id				Time series Id
 * @param aggType			Aggregation type
 * @param valueCond			Conditions on the value
 * @param timeCond			Conditions on the time composites (year, month, day, hour, minutes)
 * @param timeRanges		Time ranges conditions
 * @param groupBy			Group by clause
 * @param status			Pointer to store the status of the operation.
 * @return @s_TS_Value values or NULL for failure or zero results
 */
s_TS_Value * DTSE_TS_aggregate	(char * id, TS_aggregatedVal aggType, s_TS_Condition * valueCond,s_TS_Condition * timeCond,
									 s_TS_TimeRange * timeRanges, TS_ValueItem groupBy, DTSE_STATUS * status);



#ifdef __cplusplus
}
#endif

#endif /* TSAPI_H_ */
