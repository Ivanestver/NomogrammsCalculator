#pragma once
#include <QUuid>

namespace db_state
{
	namespace properties
	{
		const QUuid dbobject_name("8D7A1E11-C43F-4C67-BADB-888AA3F334A9");
		const QUuid nomogramm_bypassRule("581060F1-2BD0-43AA-9C80-0E9107068890");
		const QUuid nomogramm_bypassRule_List("ED6368E7-897B-4951-9705-6E7B74FD359D");
		const QUuid measure_value("8F8F8660-6AE1-4C79-8B96-0832903C5772");
		const QUuid data_type_with_unit("C3AA13ED-4620-4016-A106-7AAE653CBFD1");
		const QUuid data_output_type_with_unit("E67ABF23-9E60-49A4-BB5E-DC44F50B649E");
	}

	namespace db_ids
	{
		const QUuid ms_access("45AEBA4A-B074-47AE-B145-57E14D12345B");
		const QUuid ms_sql_server("155C3904-89AF-4451-96AC-B0171A3C24FB");
		const QUuid my_sql_server("984834A7-8026-4B83-B674-13C7EA1AB347");
	}
}