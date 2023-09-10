#include "nomogramms/Graphics.h"
#include <QVariant>
#include "db/DataBaseWrapper.h"

namespace nomogramms
{
    Graphics::Graphics(const QUuid& id)
        : base(id)
    {
        initFromDB();
    }

    Graphics::Graphics(const Graphics& other)
        : base(other)
    {
        this->parametersList = other.parametersList;
    }

    const std::vector<SMeasureUnit>& Graphics::GetParametersByType(ParameterType type) const
    {
        return parametersList.find(type)->second;
    }

    bool Graphics::operator==(const DBObject& other)
    {
        const auto& o = static_cast<const Graphics&>(other);
        return base::operator==(other)
            && this->parametersList == o.parametersList;
    }

    bool Graphics::operator!=(const DBObject& other)
    {
        return !operator==(other);
    }

    Graphics& Graphics::operator=(const DBObject& other)
    {
        if (*this == other)
            return *this;

        const auto& o = static_cast<const Graphics&>(other);
        this->parametersList = o.parametersList;

        return *this;
    }

    void Graphics::initFromDB()
    {
        auto db = db::DataBaseWrapper::GetDatabase();
        if (!db)
            return;

        QString queryString = "select [measure_unit_id] from [template_measure_unit_input] where [template_id] = ?";
        QString error;
        auto response = db->ExecuteQuery(queryString, { GetId() }, error);
        for (const auto& record : response)
        {
            auto measureUnit = std::make_shared<MeasureUnit>(record[0].toUuid());
            parametersList[ParameterType::Input].push_back(measureUnit);
        }

        queryString = "select [measure_unit_id] from [template_measure_unit_output] where [template_id] = ?";
        response = db->ExecuteQuery(queryString, { GetId() }, error);
        for (const auto& record : response)
        {
            auto measureUnit = std::make_shared<MeasureUnit>(record[0].toUuid());
            parametersList[ParameterType::Output].push_back(measureUnit);
        }
    }

    bool Graphics::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
    {
        Q_UNUSED(inputData);
        Q_UNUSED(outputData);
        Q_UNUSED(error);
        error = "Method 'Calculate' is not implemented in Graphics class";
        return false;
    }

    void Graphics::GetParameters(std::map<ParameterType, std::vector<SMeasureUnit>>& parameters) const
    {
        for (const auto& pair : parametersList)
            parameters.insert(pair);
    }
}