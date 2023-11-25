#pragma once

#include <QStyledItemDelegate>
#include <QObject>
#include <QUuid>
#include "common_macros.h"

class PropertyValueDelegate : public QStyledItemDelegate
{
	Q_OBJECT;
    DECLARE_NO_COPY_NO_MOVE(PropertyValueDelegate);

public:
    PropertyValueDelegate(QObject* parent = nullptr);
    ~PropertyValueDelegate() override = default;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    
    void setModelData(QWidget* editor, QAbstractItemModel* model,
            const QModelIndex& index) const override;

private Q_SLOTS:
    void onOpenBypassRuleListClicked();
    void onOpenMeasureUnitsClicked();

private:
    mutable bool isInput = true;
    mutable QUuid id;
};