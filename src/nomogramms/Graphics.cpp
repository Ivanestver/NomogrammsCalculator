#include "nomogramms/Graphics.h"
#include <QVariant>
#include "db/DataBaseWrapper.h"
#include "ml/NNStorage.h"

namespace nomogramms
{
    Graphics::Graphics(const QUuid& id)
        : base(id)
        , m_wrapper(m_parametersList)
    {
        initFromDB();
        QString error;
        m_wrapper.LoadNN(id, error);
    }

    const std::vector<SMeasureUnit>& Graphics::GetParametersByType(ParameterType type) const
    {
        return m_parametersList.find(type)->second;
    }

    bool Graphics::operator==(const DBObject& other) const
    {
        const auto& o = static_cast<const Graphics&>(other);
        return base::operator==(other)
            && this->m_parametersList == o.m_parametersList;
    }

    bool Graphics::operator!=(const DBObject& other) const
    {
        return !operator==(other);
    }

    void Graphics::initFromDB()
    {
        auto db = db::DataBaseWrapper::GetDatabase();
        if (!db)
            return;

        QString queryString = "select measure_unit_id from template_measure_unit_input where template_id = ?";
        QString error;
        auto response = db->ExecuteQuery(queryString, { GetId() }, error);
        for (const auto& record : response)
        {
            auto measureUnit = std::make_shared<MeasureUnit>(record[0].toUuid());
            m_parametersList[ParameterType::Input].push_back(measureUnit);
        }

        queryString = "select measure_unit_id from template_measure_unit_output where template_id = ?";
        response = db->ExecuteQuery(queryString, { GetId() }, error);
        for (const auto& record : response)
        {
            auto measureUnit = std::make_shared<MeasureUnit>(record[0].toUuid());
            m_parametersList[ParameterType::Output].push_back(measureUnit);
        }
        
        queryString = "select sub_id from template_template where master_id = ?";
        response = db->ExecuteQuery(queryString, { GetId() }, error);
        if (response.empty() || response.front().empty())
            return;

        QUuid nnId = response.front().front().toUuid();

        const auto& nnStorage = ml::NNStorage::GetInstance();
        m_network = nnStorage.GetNet(nnId);
    }

    bool Graphics::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
    {
        return m_wrapper.Calc(inputData, outputData, error);
    }

    void Graphics::GetParameters(std::map<ParameterType, std::vector<SMeasureUnit>>& parameters) const
    {
        for (const auto& pair : m_parametersList)
            parameters.insert(pair);
    }
}