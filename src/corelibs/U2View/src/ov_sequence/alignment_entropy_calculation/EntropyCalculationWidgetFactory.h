/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2022 UniPro <ugene@unipro.ru>
 * http://ugene.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include <U2Gui/OPWidgetFactory.h>

namespace U2 {

class GObjectView;

/*Widget factory for entropy calculation option panel tab*/
class U2VIEW_EXPORT EntropyCalculationWidgetFactory : public OPWidgetFactory {
    Q_OBJECT
public:
    EntropyCalculationWidgetFactory();

    QWidget* createWidget(GObjectViewController* objView, const QVariantMap& options) override;

    OPGroupParameters getOPGroupParameters() override;

    static const QString& getGroupId();

    bool passFiltration(OPFactoryFilterVisitorInterface* filter) override;

private:
    static const QString GROUP_ID;
    static const QString GROUP_ICON_STR;
    static const QString GROUP_DOC_PAGE;
};

}  // namespace U2