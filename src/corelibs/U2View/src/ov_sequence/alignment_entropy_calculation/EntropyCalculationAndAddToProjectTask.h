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

#include "EntropyCalculationTask.h"

#include <U2Core/Task.h>

namespace U2 {

/*Aligns sequence to a multiple alignment provided in the 'alignmentFilePath' argument, 
    calculates Shannon entropy, saves it to 'saveToPath' file and opens it*/

class U2VIEW_EXPORT EntropyCalculationAndAddToProjectTask : public Task {
    Q_OBJECT
public :
    EntropyCalculationAndAddToProjectTask(const QString& alignmentFilePath,
                                          const QString& saveToPath,
                                          const QString& originalFilePath,
                                          DNASequence& sequence,
                                          int chainId,
                                          bool addToProject);
    void prepare() override;
    QList<Task*> onSubTaskFinished(Task* subTask) override;

private:
    EntropyCalculationTask* entropyCalculationTask = nullptr;

    const QString alignmentFilePath;
    const QString saveToPath;
    const QString originalFilePath;
    DNASequence sequence;
    int chainId;
    const bool addToProject;
};

}  // namespace U2
