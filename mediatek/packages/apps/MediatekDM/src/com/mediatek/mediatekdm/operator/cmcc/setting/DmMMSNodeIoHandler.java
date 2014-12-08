/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

package com.mediatek.mediatekdm.operator.cmcc.setting;

import android.content.Context;
import android.net.Uri;
import android.util.Log;

import com.mediatek.mediatekdm.PlatformManager;
import com.mediatek.mediatekdm.DmConst.TAG;
import com.mediatek.mediatekdm.iohandler.DmDBNodeIoHandler;
import com.mediatek.mediatekdm.operator.cmcc.CMCCComponent;

public class DmMMSNodeIoHandler extends DmDBNodeIoHandler {

    private static final String[] ITEMS = {
            "Default", "MSCCenter"
    };
    private String[] mContentValue = {
            null, null
    };
    // In buildsystem
    // String[] mProjection = {Telephony.Carriers.MMSC};
    // Uri mTable = Telephony.Carriers.CONTENT_URI;

    // in eclipse
    private static final String[] PROJECTION = {
            "name", "mmsc"
    };

    private int mRegisteredSimId = -1;

    public DmMMSNodeIoHandler(Context ctx, Uri treeUri, String mccMnc) {
        // Log.w(PACKAGENAME, "constructed");
        mContext = ctx;
        mUri = treeUri;
        mMccMnc = mccMnc;
        for (int i = 0; i < ITEMS.length; i++) {
            mMap.put(ITEMS[i], PROJECTION[i]);
        }

    }

    /*
     * public int read(int arg0, byte[] arg1) throws MdmException {
     * Log.w(PACKAGENAME, "read called"); Log.w("haman", "mUri: " +
     * mUri.getPath()); Log.w("haman", "arg0: " + arg0); if (mRecordToRead ==
     * null) { mRecordToRead = new String(); for (int i = 0; i < item.length;
     * i++) { if (mUri.getPath().contains(item[i])) { if((String)mMap.get(item[i])
     * != null){ Cursor cur = mContext.getContentResolver().query(mTable,
     * mProjection, buildSqlString(mMccMnc), null, null); int col =
     * cur.getColumnIndex((String) mMap.get(item[i])); cur.moveToFirst();
     * mRecordToRead = cur.getString(col); cur.close(); }else{ mRecordToRead +=
     * contentValue[i]; } break; } } } if(mRecordToRead == null){ throw new
     * MdmException(0); }else{ byte[] temp = mRecordToRead.getBytes(); if (arg1
     * == null) { return temp.length; } int numberRead = 0; for (; numberRead <
     * arg1.length - arg0; numberRead++) { if (numberRead < temp.length) {
     * arg1[numberRead] = temp[arg0 + numberRead]; }else{ break; } } if
     * (numberRead < arg1.length - arg0) { mRecordToRead = null; }else
     * if(numberRead < temp.length){ mRecordToRead =
     * mRecordToRead.substring(arg1.length - arg0); } return numberRead; } }
     * public void write(int arg0, byte[] arg1, int arg2) throws MdmException {
     * Log.w(PACKAGENAME, "write was called"); Log.w("haman", "mUri: " +
     * mUri.getPath()); Log.w("haman", "arg1: " + new String(arg1));
     * Log.w("haman", "arg0: " + arg0); Log.w("haman", "arg2: " + arg2); if
     * (mRecordToWrite == null) { mRecordToWrite = new String(); } mRecordToWrite
     * += new String(arg1); if (mRecordToWrite.length() == arg2) { for (int i =
     * 0; i < item.length; i++) { if (mUri.getPath().contains(item[i])) {
     * ContentValues values = new ContentValues(); if (item[i].equals(item[2]))
     * { values.put(mProjection[0], mRecordToWrite); } else { mRecordToWrite =
     * null; break; // throw new MdmException(0); }
     * mContext.getContentResolver().update(mTable, values,
     * buildSqlString(mMccMnc), null); mRecordToWrite = null; } } } }
     */
    protected String buildSqlString(String mccMnc) {
        String mcc = mccMnc.substring(0, 3);
        String mnc = mccMnc.substring(3);
        // for cmcc
        if (mcc.equals("460") && (mnc.equals("00") || mnc.equals("02") || mnc.equals("07"))) {
            return "mcc='460' AND type='mms' AND (mnc='" + mnc + "') AND (sourcetype='0')";
        } else if (mcc.equals("460") && mnc.equals("01")) {
            return "mcc='460' AND type='mms' AND mnc='01' AND (sourcetype='0')";
        }
        // need to add some code for other operator
        return null;
    }

    @Override
    protected String[] getContentValue() {
        return mContentValue;
    }

    @Override
    protected String[] getItem() {
        return ITEMS;
    }

    @Override
    protected String[] getProjection() {
        return PROJECTION;
    }

    @Override
    protected Uri getTableToBeQueryed() {
        if (mRegisteredSimId == -1) {
            mRegisteredSimId = CMCCComponent.getRegisteredSimId(mContext);
            if (mRegisteredSimId == -1) {
                Log.e(TAG.NODEIOHANDLER, "No registered SIM ID found");
            }
        }
        return PlatformManager.getInstance().getContentUri(mRegisteredSimId);
    }

}
