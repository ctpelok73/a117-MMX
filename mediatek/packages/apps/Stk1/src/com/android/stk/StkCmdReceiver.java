/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.stk;

import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.cat.CatLog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import com.android.internal.telephony.cat.AppInterface;

/**
 * Receiver class to get STK intents, broadcasted by telephony layer.
 *
 */
public class StkCmdReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

        if (action.equals(AppInterface.CAT_CMD_ACTION)) {
            handleCommandMessage(context, intent, PhoneConstants.GEMINI_SIM_1);
        } else if (action.equals(AppInterface.CAT_CMD_ACTION_2)) {
            handleCommandMessage(context, intent, PhoneConstants.GEMINI_SIM_2);
        } if (action.equals(AppInterface.CAT_CMD_ACTION_3)) {
            handleCommandMessage(context, intent, PhoneConstants.GEMINI_SIM_3);
        } else if (action.equals(AppInterface.CAT_CMD_ACTION_4)) {
            handleCommandMessage(context, intent, PhoneConstants.GEMINI_SIM_4);
        } else if (action.equals(AppInterface.CAT_SESSION_END_ACTION)) {
            handleSessionEnd(context, intent, PhoneConstants.GEMINI_SIM_1);
        } else if (action.equals(AppInterface.CAT_SESSION_END_ACTION_2)) {
            handleSessionEnd(context, intent, PhoneConstants.GEMINI_SIM_2);
        } else if (action.equals(AppInterface.CAT_SESSION_END_ACTION_3)) {
            handleSessionEnd(context, intent, PhoneConstants.GEMINI_SIM_3);
        } else if (action.equals(AppInterface.CAT_SESSION_END_ACTION_4)) {
            handleSessionEnd(context, intent, PhoneConstants.GEMINI_SIM_4);
        }
    }

    private void handleCommandMessage(Context context, Intent intent, int sim) {
        Bundle args = new Bundle();
        int[] op = new int[2];
        op[0] = StkAppService.OP_CMD;
        op[1] = sim;
        args.putIntArray(StkAppService.OPCODE, op);
        args.putParcelable(StkAppService.CMD_MSG, intent
                .getParcelableExtra("STK CMD"));
        CatLog.d("StkCmdReceiver", "handleCommandMessage, args: " + args);
        CatLog.d("StkCmdReceiver", "handleCommandMessage, sim id: " + sim);
        Intent toService = new Intent(context, StkAppService.class);
        toService.putExtras(args);
        context.startService(toService);
    }

    private void handleSessionEnd(Context context, Intent intent, int sim) {
        Bundle args = new Bundle();
        int[] op = new int[2];
        op[0] = StkAppService.OP_END_SESSION;
        op[1] = sim;
        args.putIntArray(StkAppService.OPCODE, op);
        CatLog.d("StkCmdReceiver", "handleSessionEnd, sim id: " + sim);
        Intent toService = new Intent(context, StkAppService.class);
        toService.putExtras(args);
        context.startService(toService);
    }
}
