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

#include <U2Core/AddSequencesToAlignmentTask.h>
#include <U2Core/LoadDocumentTask.h>
#include <U2Core/MultipleSequenceAlignmentObject.h>
#include <U2Core/Task.h>

#include <U2View/AnnotatedDNAView.h>
#include <U2View/RealignSequencesInAlignmentTask.h>

namespace U2 {

/*Aligns a sequence to a multiple alignment provided in the 'alignmentFilePath' argument, calculates Shannon entropy and saves it to 'saveToPath' file*/
class U2VIEW_EXPORT EntropyCalculationTask : public Task {
    Q_OBJECT
public:
    EntropyCalculationTask(const QString& alignmentFilePath,
                           const QString& saveToPath,
                           const QString& _originalFilePath,
                           DNASequence& sequence,
                           int chainId);
    void prepare() override;
    void run() override;
    QList<Task*> onSubTaskFinished(Task* subTask) override;

private:
    void rollSequenceName();
    void calculateShannonEntropy();
    void normalizeEntropy();
    void writeEntropyToFile();

    LoadDocumentTask* loadDocumentTask = nullptr;
    AddSequenceObjectsToAlignmentTask* addSequenceTask = nullptr;
    RealignSequencesInAlignmentTask* realignSequencesTask = nullptr;
    
    MultipleSequenceAlignmentObject* alignment = nullptr;
    const QString alignmentFilePath;
    const QString saveToPath;
    const QString originalFilePath;
    DNASequence sequence;
    int chainId;
    /*sequence name will be changed if the alignment contains a sequence with the same name*/
    QString newSequenceName = "template_name";
    QVector<double> entropyForEveryColumn;
};

}  // namespace U2