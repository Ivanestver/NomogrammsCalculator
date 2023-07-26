#include "nomogramms/Graphics.h"
#include <QVariant>
#include "db/DataBaseWrapper.h"

namespace nomogramms
{
    Graphics::Graphics(const QUuid& id)
        : base(id)
        , neuralNetwork(nullptr)
    {}

    Graphics::Graphics(const Graphics& other)
        : base(other)
    {
        this->parametersList = other.parametersList;
    }

    const std::vector<SMeasure> Graphics::GetParametersByType(ParameterType type) const
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
        const auto* db = db::DataBaseWrapper::GetInstance();

        QString queryString = "select slave_id, master_slave_value from template_template where master_id = :1";
        std::vector<QVariant> params{QVariant(GetId())};
        QString error = "";
        auto result = db->ExecuteQuery(queryString, params, error);
        if (!error.isEmpty())
            return;

        for (const auto& v : result)
        {
            ParameterType type = (ParameterType)v[1].value<int>();
            auto measure = std::make_shared<Measure>(v[0].value<QUuid>());
            parametersList[type].push_back(measure);
        }
    }

    bool Graphics::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
    {
        error = "Method 'Calculate' is not implemented in Graphics class";
        return false;
    }

    void Graphics::GetParameters(std::map<ParameterType, std::vector<SMeasure>>& parameters) const
    {
        for (const auto& pair : parameters)
            parameters.insert(pair);
    }
}