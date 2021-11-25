/********************************************************************************
 * Schneider-Electric                                                           *
 * Global Solutions - Digital Services Transformation                           *
 * Digital Services Platform                                                    *
 * Copyright (c) 2014 - All rights reserved.                                    *
 *                                                                              *
 * Developed by:                                                                *
 *          Hicham Hossayni                                                     *
 *                                                                              *
 * No part of this document may be reproduced in any form without the           *
 * express written consent of Schneider-Electric.                               *
 ********************************************************************************/

/**
 * @file
 * Before using DTSE brick, the integrator has to implement the Data Model
 * abstract layer as defined in this header file to allow DTSE access
 * to the Data Model managed by the hosting gateway.<br>
 *
 * @version 2.0
 *
 * <b>Modification history</b>
@verbatim
 +-------------+---------------+----------------------------------------------+
 |     Date    |    Author     |           Change Description                 |
 +-------------+---------------+----------------------------------------------+
 |             |               |                                              |
 +-------------+---------------+----------------------------------------------+
@endverbatim
 */


#ifndef DMAPI_H_
#define DMAPI_H_

#include "DTSE_errorCodes.h"
#include "DTSE_AL.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
                              Defines
==============================================================================*/

/**
 * DM_SUPPORTS_NOTIFY_ON_CHANGE indicates to the DTSE that the DMAPI implementation
 *   supports the notification on change mechanism (function : DM_NotifyOnChange).
 *   if not defined, the DTSE will make periodic calls to the DMAPI functions to check if
 *   there are changes on the values of the monitored variables.
 *
 * @attention This feature is not supported by the DTSE version 1.0
*/
#define DM_SUPPORTS_NOTIFY_ON_CHANGE 1


/*=============================================================================
                              Enumerations
==============================================================================*/


/**
 * Enumeration of supported types for variables values
 */
typedef enum
{

	TYPE_INVALID =-0x01,	/**<  Invalid type */
	TYPE_FLOAT   = 0x01,	/**<  Float type */
	TYPE_LONG    = 0x02,	/**<  Long type */
	TYPE_INT     = 0x03,	/**<  Integer type */
	TYPE_CHAR    = 0x04,	/**<  Character type */
	TYPE_STR     = 0x05,	/**<  String type */
	TYPE_BOOL    = 0x06,	/**<  Boolean type */
	TYPE_INT_16  = 0x07,	/**<  Signed integer (16 bits) */
	TYPE_UINT_8  = 0x08,	/**<  Unsigned integer (8 bits) */
	TYPE_UINT_16 = 0x09,	/**<  Unsigned integer (16 bits) */
	TYPE_UINT_32 = 0x0A,	/**<  Unsigned integer (32 bits) */
	TYPE_UINT_64 = 0x0B,	/**<  Unsigned integer (64 bits) */
	TYPE_BLOB    = 0x0C		/**<  BLOB type, the size of this type must be specified in the s_Variable structure */
}variable_type;



/**
 * Enumeration of the nodes types managed by DMAPI,
 */
typedef enum
{

	DEVICE_NODE  	= 0x10,		/**<  Device node : represents a real node having at least one capability (Sensor or actuator). @see s_Device. */
	VARIABLE_NODE   = 0x20,		/**<  Variable node : defines a real capability (Sensor or actuator). @see s_Variable. */
	TAG_NODE		= 0x30,		/**<  Tag node : defines a property that can be attached to a Device or a Variable node. @see s_Tag. */
	ID_NODE			= 0x40		/**<  Identifier node : small structure to store the identifier of a Device or Variable node. @see s_NodeId. */
} node_type;


/*=============================================================================
                              Structures
==============================================================================*/

/**
 * Structure to store the identifier of a DMAPI node (device or variable)
 *
 * For detailed information, see struct NodeId_struct.
 */
typedef struct NodeId_struct      s_NodeId;

/**
 * @see s_NodeId
 */
struct NodeId_struct
{
	char *			id;		/**<  identifier of the node (variable or device) */
	s_NodeId * 		next;	/**<  Pointer on the next identifier in linked list */
};



/**
 * Tag structure : contain the tags information to be attached to devices or variables.<br>
 * Example:
 * @code
 * // To define the tag "usage:Measurement", the structure will have this format
 * s_Tag usage = {.Namespace = "usage" , .instance = "Measurement", .next = NULL};
 *
 * // Also, "protocol:Modbus" will be defined by
 * s_Tag protocol = {.Namespace = "protocol" , .instance = "Modbus", .next = NULL};
 * @endcode
 */
typedef struct Tag_struct 		s_Tag;

/**
 * @see s_Tag
 */
struct Tag_struct
{

	char *  	Namespace;		/**<  Tag's name space */
	char *		instance;		/**<  Tag's instance */
	s_Tag * 	next;			/**<  Pointer on the next Tag to create linked lists */
};

/**
 * @brief The Variable's structure: defines the information relative to DMAPI variables.
 *
 * DMAPI variable is defined to represent real sensor or actuator in the data model.
 * It is charcterized with an indentifier, name, unit, value, ...etc.
 * It can also have one or more Tags that describe the capability, and give any other kind of
 * important information (eg. usage, refresh frequency, hardware version, ...). <br>
 * The <i>value</i> field is defined as <i>void *</i> to store any type of information comming from the sensor/actuator.
 * For instance, the information can be measurement of a sensor or the state of an actuator.
 *
 * @note A DMAPI variable reflects one and only one capability (sensor or actuator),
 * and MUST have one device parent.<br>
 * DMAPI variables are identified with the couple <id , parent_deviceId>,
 * so the variable's id is not necessary unic in the data model.
 *
 * <b>Example:</b>A Temperature sensor having the tag "usage:Temperature" and measuring 15.0 °C can be defined as a DMAPI variable with :
 *
 * @code
 *
 * // 1) We define the Tag structure
 * s_Tag 	  TemperatureTag = { .Namespace = "usage",   .instance = "Temperature",  .next = NULL};
 *
 * // 2) We define the variable's value
 * float 	  TemperatureValue = 15.0;
 *
 * // 3) Finally, we create th variable structure
 * s_Variable TemperatureVariable = {
 * 					.id 			 =  "TemperatureID",
 * 					.name			 =  "Temperature Sensor",
 * 					.unit			 =  "Celcius", // or "C", "F" ....
 * 					.parent_deviceId =  "MultiSensorDevice",
 * 					.tags			 =  &TemperatureTag,
 * 					.type			 =  TYPE_FLOAT,
 * 					.value			 =  &TemperatureValue,
 * 					.size			 =  sizeof ( float ),
 * 					.next			 =  NULL;
 * 				};
 *
 * @endcode
 */
typedef struct Variable_struct 	s_Variable;

/**
 * @see s_Variable
 */
struct Variable_struct
{

	char *			id;					/**<  Identifier of the variable. */
	char *			name;				/**<  Name of the variable */
	char *  		unit;				/**<  Unit */
	char *  	    parent_deviceId;	/**<  the id of the parent device of the variable */
	s_Tag *			tags;				/**<  Chained list of variable's tags */
	variable_type 	type;				/**<  the type of the variable */
	void *  	    value;				/**<  a pointer on the variable's value, must be casted to the suitable type before use */
	DTSE_size        size;				/**<  size of the value, necessary for the BLOB type, it corresponds to the number of bytes of value */
	s_Variable * 	next;				/**<  Pointer on the next Variable in a linked list. */
};



/**
 * @brief The Device structure: defines the information relative to DMAPI devices.
 *
 * DMAPI device is defined to represent a real device embedding or managing at least one capability
 * (sensor or actuator) that can be represented by a DMAPI Variable.
 * It is charcterized with an indentifier, name, tags, the list of managed capabilities, ...<br>
 * Each device can have one or more Tags that describe the device, and give any other kind of
 * important information (eg. location, protocol, hardware, ...). <br>
 * The s_Device structure allows also to define hierarchical relations between devices.
 * This can be achieved by defining the parent device and the children devices. The next pointer
 * is used to target devices with the same parent.<br>
 *
 * @note DTSE needs at least one device to be defined in the data model. In case of a data model containing just Variables,
 * the integrator has to define one <i>virtual</i> device which will be considered as the parent of
 * all DMAPI variables.
 * DMAPI devices are identified by the <i>id</i> attribute, so the id must be unic in the data model
 *
<b>Example:</b>Let's consider a device managing two capabilities : Temperature and Humidity. Such device can be defined by:
 *
 * @code
 *
 * 1.1) We assume that the variables are already created with the corresponding tags and attributes (see struct Variable_struct example)
 * s_Variable Temperature, Humidity;
 *
 * s_NodeId   TemperatureId = {Temperature.id , NULL},
 *            HumidityId = {Humidity.id, NULL};
 *
 * 1.2) We link between the two identifiers to create a chained list
 * s_NodeId   *VariablesList = &TemperatureId; 						// chained list of Variables IDs
 * VariablesList -> next = &HumidityId;
 *
 * 2) We define the Tag structure
 * s_Tag 	  deviceTag = { .Namespace = "location",   .instance = "Paris",  .next = NULL};
 *
 * 3) We define the device structure
 * s_Device device = {
 * 			.id                 =  "device_Id",
 * 			.name               =  "device_Name",
 * 			.parentId           =  NULL, // or "device_parent_Id" if it exists
 * 			.tags               =  &deviceTag,
 * 			.variables          =  VariablesList,
 * 			.children           =  NULL, // or a chained list of children identifiers if they exist
 * 			.next               =  NULL,
 *
 * 		};
 * @endcode
 */
typedef struct Device_struct 		s_Device;

/**
 * @see s_Device
 */
struct Device_struct
{
	char *			id;				/**<  identifier of the device */
	char *  		name;			/**<  name of the device */
	char *          parentId;      	/**<  ID of the device's parent */
	s_Tag *			tags;			/**<  list of device's tags */
	s_NodeId *	    variables;      /**<  list of variables IDs attached to the device */
	s_Device * 		children;		/**<  list of device's children (must be devices) */
	s_Device * 		next;			/**<  next device (brother) */
};


/*==============================================================================
        					Function Definitions
 ==============================================================================*/

/**
 * @brief DMAPI Open, used to initialize the necessary global resources needed by
 *        the DMAPI functions like global variables, or database connection, ...
 *        It is called once by the DTSE_Init() function at the starting time.
 *        NB : this implementation supports only one instance initialization.
 *
 * @return DTSE_SUCCESS on success or a negative value for error (see DTSE_errorCodes.h)
 */
DTSE_STATUS			DM_Open(void);


/**
 * @brief DMAPI Open, used to free the resources initialized by DM_Open
 *        It is called once by the DTSE_Close() function when the application is
 *        shutting down.
 *
 * @return DTSE_SUCCESS on success or a negative value for error (see DTSE_errorCodes.h)
 */
DTSE_STATUS				DM_Close(void);


/**
 * @brief Return the Device's structure identified by the input id.
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] Id : Identifier of the device
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return NULL or the s_Device structure
 */
s_Device *   	DM_GetDevice  ( char *Id , DTSE_STATUS * Status);


/**
 * @brief Return the list of the variables IDs attached to the device identified by deviceId.
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the device
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return NULL or a chained list of s_NodeId
 */
s_NodeId *	DM_GetDeviceVariablesId( char *deviceId, DTSE_STATUS * Status);

/**
 * @brief Return the list of the tags attached to a device with deviceId
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the device
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return NULL or a chained list of s_Tag
 */
s_Tag *	DM_GetDeviceTags( char *deviceId, DTSE_STATUS * Status);

/**
 * @brief Return the identifier of the device's parent, this function must allocate a
 * 		  dynamic memory which will contain the parent's identifier.
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the device
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return the string of the device's parent ID or NULL if not found (root node)
 */
char *	DM_GetDeviceParentId( char *deviceId, DTSE_STATUS * Status);

/**
 * @brief Return the identifiers of all devices having the input deviceId as parent
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the device
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return a chained list of s_NodeId, or NULL if the device have no children
 */
s_NodeId *	DM_GetDeviceChildrenId( char *deviceId, DTSE_STATUS * Status);

/**
 * @brief Return the Variable structure identified by input deviceId and variableId,
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the variable's parent
 * @param[in] variableId : Identifier of the variable
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return NULL or a s_Variable structure
 */
s_Variable * 	DM_GetVariable( char *deviceId, char *variableId , DTSE_STATUS * Status);

/**
 * @brief Return the value type of the Variable identified by input deviceId and variableId,
 *
 * @param[in] deviceId : Identifier of the variable's parent
 * @param[in] variableId : Identifier of the variable
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return a variable type
 */
variable_type	DM_GetVariableType( char *deviceId, char *variableId , DTSE_STATUS * Status);

/**
 * @brief Return the value of the Variable identified by input deviceId and variableId,
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the variable's parent
 * @param[in] variableId : Identifier of the variable
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return a variable value
 */
void *	DM_GetVariableValue( char *deviceId, char *variableId, DTSE_STATUS * Status);

/**
 * @brief Return the list of tags attached to the variable identified by input deviceId and variableId,
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] deviceId : Identifier of the variable's parent
 * @param[in] variableId : Identifier of the variable
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return a chained list of tags
 */
s_Tag * DM_GetVariableTags(char *deviceId, char *variableId, DTSE_STATUS * Status);

/**
 * @brief Return a List of devices matching at least one of the input tags
 *        each device is returned with its list of tags and its list of variables Identifiers.
 *        The next field is used to point to the next device in the list.
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] listOfTags : a list of tags
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return a chained list of s_Device structured like in the following example :
 *
 * @verbatim
        results
          ║
          ║
          ╠═ Device 1 ┄┄┄┄┄> tag1 ┄┄┄┄┄>  tag2 ┄┄┄┄┄> ... NULL
          ║    ┊
          ║    └┄┄┄┄┄> Variable ID1 ┄┄┄┄┄> Variable ID2 ┄┄┄┄┄> ... NULL
          ║
          ║
          ╠═ Device 2 ┄┄┄┄┄> tag1 ┄┄┄┄┄>  tag2 ┄┄┄┄┄> ... NULL
          ║    ┊
          ║    └┄┄┄┄┄> Variable ID1 ┄┄┄┄┄> Variable ID2 ┄┄┄┄┄> ... NULL
          ╻
          ╻
          ╻
          V
        NULL
  @endverbatim
 */

s_Device *   	DM_GetDevices_ByTags (s_Tag * listOfTags , DTSE_STATUS * Status);



/**
 * @brief Returns a list of variables matching at least one of the input tags
 *        each variable is returned with its list of tags
 * 		  the next field is used to point to the next variable in the list.
 * @note  Note that the allocated memory by this function will be freed by the DTSE brick.
 *
 * @param[in] listOfTags : Chained list of tags
 * @param[out] Status : Non NULL pointer to store the status of the operation (see DTSE_errorCodes.h)
 *
 * @return 	list of s_Variable structured like in the following example :
 * @verbatim
        results
          ║
          ║
          ╠═ Variable 1 ┄┄┄┄┄> tag1 ┄┄┄┄┄>  tag2 ┄┄┄┄┄> ... NULL
          ║
          ╠═ Variable 2 ┄┄┄┄┄> tag1 ┄┄┄┄┄>  tag2 ┄┄┄┄┄> ... NULL
          ║
          ╠═ Variable 3 ┄┄┄┄┄> tag1 ┄┄┄┄┄>  tag2 ┄┄┄┄┄> ... NULL
          ║
          ╻
          ╻
          V
        NULL
  @endverbatim
 */
s_Variable*   	DM_GetVariables_ByTags(s_Tag * listOfTags , DTSE_STATUS * Status);

/**
 * @brief Set the value of a DMAPI variable. The DTSE will check the type of the variable before
 * calling the DM_SetVariable function to assign the right value type to the corresponding variable.<br>
 * @note Note that DTSE will allocate dynamically the memory fr the new value. Take care of releasing this memory after use.
 *
 * @param[in] deviceId : Identifier of the variable's parent
 * @param[in] variableId : Identifier of the variable
 * @param[in] value : Pointer on the new (allocated) value.
 *
 * @return DTSE_SUCCESS on success or a negative value for error (see DTSE_errorCodes.h)
 */
DTSE_STATUS DM_SetVariable( char *deviceId, char *variableId, void * value );


/**
 * @brief This function frees the memory of different DMAPI nodes types (ex. s_Device, s_Variable ...)
 *        Note that DTSE will make calls to this function to free all allocated memory by DMAPI.
 *
 * Example : The C code below is designed to be memory-neutral, it releases all allocated
 * 		  memory by the "DM_get.." functions.
 * @code
 * 	DTSE_QueryResult * results = DTSE_Query("Search .....");
 *
 * 	// process results
 * 	// ...
 *
 * 	DTSE_DeleteQueryResult(results);
 * @endcode
 *
 * @note <b>Special case</b> : When a "DM_get..." function returns a pointer on a global variable,
 * 		  DTSE will anyway try to free it by calling "DM_FreeNode()".
 * 		  So the integrator has to manage this case by keeping the global variables in memory until
 * 		  the "DM_Close_Query_Session()" or "DM_Close()" calls.
 *
 * @param[in] type      : the type of the node to be free'd
 * @param[in] nodePtr   : the pointer on the node to free
 * @param[in] recursive : In case of chained list, recursive = 0 indicates that we need to free only the node
 * 					  on the given pointer. Otherwise (recursive != 0), the function must release also all the next nodes
 * 					  in the chained list,
 * @return DTSE_SUCCESS on success or a negative value for error (see DTSE_errorCodes.h)
 */
DTSE_STATUS DM_FreeNode(node_type type, void * nodePtr, DTSE_int recursive);


/**
 * @brief Opens a Query session, it allocates the necessary resources to process a single query (database connection, file open,
 *        lock shared resources, ...). This function is called once before each DTSE query process.
 *
 * @return sessionId >= 0 to when ready to process a query, or a negative value if not ready.
 */
DTSE_int DM_Open_Query_Session();

/**
 * @brief Closes a Query session, it is called by the DTSE after a query process finishes.
 *        It allows the implementer to free the allocated resources by the DM_Open_Query_Session() function.
 *
 * @param[in] sessionId retrieved from DM_Open_Query_Session
 *
 * @return DTSE_SUCCESS on success or a negative value for error (see DTSE_errorCodes.h)
 */
DTSE_int DM_Close_Query_Session(DTSE_int sessionId);

/**
 * @brief Notifies the DTSE when a device has been added or removed or a tag has changed
 * It will be called by the implementer when a change takes place. 
 * This will allow the DTSE to update its local cache.
 * @note The cache is not a mirror of your data model but a list of Ids and Tags
 *
 * @attention This feature is not supported by the DTSE Version 1
 *
 * @param[in] deviceId : The identifier of the device to observe and notify when a change has occurred.
 * @param[in] variable_id : When variable_id is NULL, this means that DTSE subscribes for a
 * 						notification on a device change. Otherwise, it is the variable Id where the change took place.
 * @param[in] pfn : call back function that must be called by dmapi when a change has occurred
 *
 * @return DTSE_SUCCESS on success or a negative value for error (see DTSE_errorCodes.h)
 */
DTSE_STATUS DM_NotifyOnChange( char * deviceId,
							  char * variable_id,
							  void (*pfn)(char * deviceID, char *variableID) );

#ifdef __cplusplus
}
#endif

#endif /* DMAPI_H_ */
