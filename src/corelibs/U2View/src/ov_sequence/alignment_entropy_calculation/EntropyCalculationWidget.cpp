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

#include <QRegularExpression> 

#include "EntropyCalculationWidget.h"

#include <U2Core/AppContext.h>
#include <U2Core/AppSettings.h>
#include <U2Core/BaseDocumentFormats.h>
#include <U2Core/FileFilters.h>
#include <U2Core/L10n.h>
#include <U2Core/UserApplicationsSettings.h>
#include <U2Core/U2OpStatusUtils.h>
#include <U2Core/DNASequenceObject.h>
#include <U2View/ADVSequenceObjectContext.h>

#include <U2Gui/LastUsedDirHelper.h>
#include <U2Gui/SaveDocumentController.h>
#include <U2Gui/ShowHideSubgroupWidget.h>
#include <U2Gui/U2FileDialog.h>

#include <U2View/AnnotatedDNAView.h>

namespace U2 {

EntropyCalculationWidget::EntropyCalculationWidget(AnnotatedDNAView* _annotatedDnaView)
    : annotatedDnaView(_annotatedDnaView) {
    setupUi(this);
    initLayout();
    initSaveController();
    runButton->setEnabled(false);

    connect(alignmentToolButton, SIGNAL(clicked()), SLOT(sl_onFileSelectorClicked()));
    connect(runButton, SIGNAL(clicked()), SLOT(sl_onRunButtonClicked()));
    connect(alignmentLineEdit, SIGNAL(textChanged(QString)), SLOT(sl_onTextChanged()));
}

void EntropyCalculationWidget::initLayout() {
    additionalSettingsLayout->addWidget(new ShowHideSubgroupWidget(
        tr("Additional settings"), tr("Additional settings"), additionalSettingsWidget, true));
}

void EntropyCalculationWidget::initSaveController() {
    SaveDocumentControllerConfig conf;
    conf.fileDialogButton = saveToToolButton;
    conf.fileNameEdit = saveToLineEdit;
    conf.parentWidget = this;
    conf.saveTitle = tr("Save file");
    conf.defaultFormatId = BaseDocumentFormats::PLAIN_PDB;

    //get save file name from AnnotatedDnaView
    auto name = annotatedDnaView->getActiveSequenceContext()->getSequenceObject()->getDocument()->getName();
    conf.defaultFileName = AppContext::getAppSettings()->getUserAppsSettings()->getDefaultDataDirPath() + "/" + name;
    saveController = new SaveDocumentController(conf, {BaseDocumentFormats::PLAIN_PDB}, this);
}

void EntropyCalculationWidget::sl_onFileSelectorClicked() { 
    LastUsedDirHelper lod("ENTROPY_CALCULATION_LAST_DIR");
    QString filter = FileFilters::createFileFilterByObjectTypes({GObjectTypes::MULTIPLE_SEQUENCE_ALIGNMENT}, true);
    lod.url = U2FileDialog::getOpenFileName(QApplication::activeWindow(), tr("Select file to open..."), lod.dir, filter);
    if (!lod.url.isEmpty()) {
        alignmentLineEdit->setText(lod.url);
    }
}

void EntropyCalculationWidget::sl_onRunButtonClicked() {
    auto sequenceObj = annotatedDnaView->getActiveSequenceContext()->getSequenceObject();
    auto originalFilePath = sequenceObj->getDocument()->getURLString();

    U2OpStatus2Log os;
    auto sequence = sequenceObj->getWholeSequence(os);
    CHECK_OP(os, );
    int chainId = sequenceObj->getSequenceInfo().value("CHAIN_ID").toInt();
    auto loadTask = new EntropyCalculationAndAddToProjectTask(alignmentLineEdit->text(), saveToLineEdit->text(), 
        originalFilePath, sequence, chainId, addToProjectCheckBox->isChecked());
    AppContext::getTaskScheduler()->registerTopLevelTask(loadTask);
}

void EntropyCalculationWidget::sl_onTextChanged() {
    runButton->setEnabled(!alignmentLineEdit->text().isEmpty());
}

}  // namespace U2