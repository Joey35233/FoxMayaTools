#include "RigArmDef.h"

#include <cmath>

namespace fox::anim::Experimental
{
    using undefined = ubyte;
    using undefined2 = ushort;
    using undefined4 = uint;
    using undefined8 = ulong;

    using longlong = long long;
    using ulonglong = ulong;

    void SolveTwoBoneIK(Quat* out_upperArm_rgr, Quat* out_lowerArm_rgr, Vector3* lowerArm_blp, Vector3* effector_blp, Vector3* chainPlaneNormal, Vector3* upperArm_rgp, Vector3* effector_rgp, Vector3* chain_uv)
    {
        uarm2eff_rv = effector_rgp - upperArm_rgp;

        // Normalized vectors
        lowerArm_buv = normalize(lowerArm_blp);
        effector_buv = normalize(effector_blp);
        uarm2eff_ruv = normalize(uarm2eff_rv);

        // Lengths
        lowerArm_blp_len2 = dot(lowerArm_blp, lowerArm_blp);
        effector_blp_len2 = dot(effector_blp, effector_blp);
        uarm2eff_rv_len2 = dot(uarm2eff_rv, uarm2eff_rv);

        lowerArm_blp_len = sqrt(lowerArm_blp_len2);
        effector_blp_len = sqrt(effector_blp_len2);
        uarm2eff_rv_len = sqrt(uarm2eff_rv_len2);

        uarm2eff_rv_norm = 1.0f / uarm2eff_rv_len;

        // UpperArm
        basisA_uv = cross(chain_uv, uarm2eff_ruv);

        weirdLen = ((lowerArm_blp_len2 - effector_blp_len2) + uarm2eff_rv_len2) * 0.5f * uarm2eff_rv_norm;
        weirdLen = maxps(weirdLen, 0);

        maskedWeirdLen = (uarm2eff_rv_len < effector_blp_len + lowerArm_blp_len) ? weirdLen : lowerArm_blp_len;
        maskedWeirdLen2 = maskedWeirdLen * maskedWeirdLen;
        maskedWeirdDist2 = lowerArm_blp_len2 - maskedWeirdLen2;
        clampedMaskedWeirdDist2 = max(maskedWeirdDist2, 0);
        packedWeirdDist = sqrtps(auVar2, clampedMaskedWeirdDist2);

        upperArm_basisB_v = maskedWeirdLen * uarm2eff_ruv + packedWeirdDist * chain_uv;
        upperArm_basisB_uv = normalize(upperArm_basisB_v);

        basisC_uv = cross(upperArm_basisB_uv, basisA_uv);

        lowerArmCrossNormal = cross(lowerArm_buv, chainPlaneNormal);

        m00 = chainPlaneNormal.x * basisA_uv.x + lowerArm_buv.x * upperArm_basisB_uv.x + lowerArmCrossNormal.x * basisC_uv.x;
        m01 = chainPlaneNormal.x * basisA_uv.y + lowerArm_buv.x * upperArm_basisB_uv.y + lowerArmCrossNormal.x * basisC_uv.y;
        m02 = chainPlaneNormal.x * basisA_uv.z + lowerArm_buv.x * upperArm_basisB_uv.z + lowerArmCrossNormal.x * basisC_uv.z;
        m10 = chainPlaneNormal.y * basisA_uv.x + lowerArm_buv.y * upperArm_basisB_uv.x + lowerArmCrossNormal.y * basisC_uv.x;
        m11 = chainPlaneNormal.y * basisA_uv.y + lowerArm_buv.y * upperArm_basisB_uv.y + lowerArmCrossNormal.y * basisC_uv.y;
        m12 = chainPlaneNormal.y * basisA_uv.z + lowerArm_buv.y * upperArm_basisB_uv.z + lowerArmCrossNormal.y * basisC_uv.z;
        m20 = chainPlaneNormal.z * basisA_uv.x + lowerArm_buv.z * upperArm_basisB_uv.x + lowerArmCrossNormal.z * basisC_uv.x;
        m21 = chainPlaneNormal.z * basisA_uv.y + lowerArm_buv.z * upperArm_basisB_uv.y + lowerArmCrossNormal.z * basisC_uv.y;
        m22 = chainPlaneNormal.z * basisA_uv.z + lowerArm_buv.z * upperArm_basisB_uv.z + lowerArmCrossNormal.z * basisC_uv.z;

        //// Matrix to quaternion (https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/)
        //// Or this: https://github.com/glampert/vectormath/blob/master/sse/matrix.hpp
        ////          https://github.com/glampert/vectormath/blob/master/scalar/matrix.hpp
        //trace = m11 + m00 + m22;
        //auVar43._0_4_ = ((m00 - m11) - m22) + 1.0f;
        //auVar43._4_4_ = ((m11 - m22) - m00) + 1.0f;
        //auVar43._8_4_ = ((m22 - m00) - m11) + 1.0f;
        //auVar43._12_4_ = m11 + m00 + m22 + 1.0f;
        //auVar2 = rsqrtps(auVar15, auVar43);
        //fVar35 = auVar2._0_4_;
        //fVar36 = auVar2._4_4_;
        //fVar10 = auVar2._8_4_;
        //fVar1 = auVar2._12_4_;
        //fVar5 = m21 + m12;
        //fVar8 = m02 + m20;
        //fVar6 = m10 + m01;
        //fVar31 = m12 - m21;
        //fVar19 = m20 - m02;
        //fVar22 = m01 - m10;
        //fVar39 = (3.0 - fVar35 * auVar43._0_4_ * fVar35) * fVar35 * 0.5f * 0.5f;
        //fVar41 = (3.0 - fVar36 * auVar43._4_4_ * fVar36) * fVar36 * 0.5f * 0.5f;
        //fVar37 = (3.0 - fVar10 * auVar43._8_4_ * fVar10) * fVar10 * 0.5f * 0.5f;
        //fVar42 = (3.0 - fVar1 * auVar43._12_4_ * fVar1) * fVar1 * 0.5f * 0.5f;
        //local_158 = -(uint)(m11 < m22 && m00 < m22);
        //uStack_154 = -(uint)(m11 < m22 && m00 < m22);
        //uStack_150 = -(uint)(m11 < m22 && m00 < m22);
        //uStack_14c = -(uint)(m11 < in_xmmTmp2_Dd && m00 < in_xmmTmp2_Dd);
        //out_upperArm_gr->x =
        //    (float)(~- (uint)(0.0 < trace) &
        //        (~local_158 &
        //            (~- (uint)(m00 < m11) & (uint)(auVar43._0_4_ * fVar39) |
        //                (uint)(fVar6 * fVar41) & -(uint)(m00 < m11)) | (uint)(fVar8 * fVar37) & local_158) |
        //        (uint)(fVar31 * fVar42) & -(uint)(0.0 < trace));
        //out_upperArm_gr->y =
        //    (float)(~- (uint)(0.0 < trace) &
        //        (~uStack_154 &
        //            (~- (uint)(m00 < m11) & (uint)(fVar6 * fVar39) |
        //                (uint)(auVar43._4_4_ * fVar41) & -(uint)(m00 < m11)) | (uint)(fVar5 * fVar37) & uStack_154) |
        //        (uint)(fVar19 * fVar42) & -(uint)(0.0 < trace));
        //out_upperArm_gr->z =
        //    (float)(~- (uint)(0.0 < trace) &
        //        (~uStack_150 &
        //            (~- (uint)(m00 < m11) & (uint)(fVar8 * fVar39) | (uint)(fVar5 * fVar41) & -(uint)(m00 < m11)) |
        //            (uint)(auVar43._8_4_ * fVar37) & uStack_150) | (uint)(fVar22 * fVar42) & -(uint)(0.0 < trace));
        //out_upperArm_gr->w =
        //    (float)(~- (uint)(0.0 < in_xmmTmp2_Dd) &
        //        (~uStack_14c &
        //            (~- (uint)(m00 < in_xmmTmp2_Dd) & (uint)(fVar31 * fVar39) |
        //                (uint)(fVar19 * fVar41) & -(uint)(m00 < in_xmmTmp2_Dd)) | (uint)(fVar22 * fVar37) & uStack_14c) |
        //        (uint)(auVar43._12_4_ * fVar42) & -(uint)(0.0 < in_xmmTmp2_Dd));

        // LowerArm
        lowerArm_basisB_v = uarm2eff_rv - upperArm_basisB_v;
        lowerArm_basisB_uv = normalize(lowerArm_basisB_v);

        basisC_uv = cross(lowerArm_basisB_uv, basisA_uv);

        effectorCrossNormal = cross(effector_buv, chainPlaneNormal);

        m00 = chainPlaneNormal.x * basisA_uv.x + effector_buv.x * lowerArm_basisB_uv.x + effectorCrossNormal.x * basisC_uv.x;
        m01 = chainPlaneNormal.x * basisA_uv.y + effector_buv.x * lowerArm_basisB_uv.y + effectorCrossNormal.x * basisC_uv.y;
        m02 = chainPlaneNormal.x * basisA_uv.z + effector_buv.x * lowerArm_basisB_uv.z + effectorCrossNormal.x * basisC_uv.z;
        m10 = chainPlaneNormal.y * basisA_uv.x + effector_buv.y * lowerArm_basisB_uv.x + effectorCrossNormal.y * basisC_uv.x;
        m11 = chainPlaneNormal.y * basisA_uv.y + effector_buv.y * lowerArm_basisB_uv.y + effectorCrossNormal.y * basisC_uv.y;
        m12 = chainPlaneNormal.y * basisA_uv.z + effector_buv.y * lowerArm_basisB_uv.z + effectorCrossNormal.y * basisC_uv.z;
        m20 = chainPlaneNormal.z * basisA_uv.x + effector_buv.z * lowerArm_basisB_uv.x + effectorCrossNormal.z * basisC_uv.x;
        m21 = chainPlaneNormal.z * basisA_uv.y + effector_buv.z * lowerArm_basisB_uv.y + effectorCrossNormal.z * basisC_uv.y;
        m22 = chainPlaneNormal.z * basisA_uv.z + effector_buv.z * lowerArm_basisB_uv.z + effectorCrossNormal.z * basisC_uv.z;

        //// Matrix to quaternion (https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/)
        //// Or this: https://github.com/glampert/vectormath/blob/master/sse/matrix.hpp
        ////          https://github.com/glampert/vectormath/blob/master/scalar/matrix.hpp
        //trace = m11 + m00 + m22;
        //auVar40._0_4_ = ((m00 - m11) - m22) + 1.0f;
        //auVar40._4_4_ = ((m11 - m22) - m00) + 1.0f;
        //auVar40._8_4_ = ((m22 - m00) - m11) + 1.0f;
        //auVar40._12_4_ = m11 + m00 + m22 + 1.0f;
        //auVar2 = rsqrtps(auVar16, auVar40);
        //uVar44 = -(uint)(m11 < m22 && m00 < m22);
        //uVar45 = -(uint)(m11 < m22 && m00 < m22);
        //uVar46 = -(uint)(m11 < m22 && m00 < m22);
        //uVar47 = -(uint)(m11 < in_xmmTmp2_Dd && m00 < in_xmmTmp2_Dd);
        //fVar35 = auVar2._0_4_;
        //fVar36 = auVar2._4_4_;
        //fVar10 = auVar2._8_4_;
        //fVar1 = auVar2._12_4_;
        //fVar5 = m21 + m12;
        //fVar7 = m02 + m20;
        //fVar13 = m10 + m01;
        //fVar9 = m12 - m21;
        //fVar37 = m20 - m02;
        //fVar25 = m01 - m10;
        //local_148 = (3.0 - fVar35 * auVar40._0_4_ * fVar35) * 0.5f * fVar35 * 0.5f;
        //fStack_144 = (3.0 - fVar36 * auVar40._4_4_ * fVar36) * 0.5f * fVar36 * 0.5f;
        //fStack_140 = (3.0 - fVar10 * auVar40._8_4_ * fVar10) * 0.5f * fVar10 * 0.5f;
        //fStack_13c = (3.0 - fVar1 * auVar40._12_4_ * fVar1) * 0.5f * fVar1 * 0.5f;
        //out_lowerArm_gr->x =
        //    (float)(~- (uint)(0.0 < trace) &
        //        (~uVar44 & (~- (uint)(m00 < m11) & (uint)(auVar40._0_4_ * local_148) |
        //            (uint)(fVar13 * fStack_144) & -(uint)(m00 < m11)) | (uint)(fVar7 * fStack_140) & uVar44)
        //        | (uint)(fVar9 * fStack_13c) & -(uint)(0.0 < trace));
        //out_lowerArm_gr->y =
        //    (float)(~- (uint)(0.0 < trace) &
        //        (~uVar45 & (~- (uint)(m00 < m11) & (uint)(fVar13 * local_148) |
        //            (uint)(auVar40._4_4_ * fStack_144) & -(uint)(m00 < m11)) |
        //            (uint)(fVar5 * fStack_140) & uVar45) | (uint)(fVar37 * fStack_13c) & -(uint)(0.0 < trace));
        //out_lowerArm_gr->z =
        //    (float)(~- (uint)(0.0 < trace) &
        //        (~uVar46 & (~- (uint)(m00 < m11) & (uint)(fVar7 * local_148) |
        //            (uint)(fVar5 * fStack_144) & -(uint)(m00 < m11)) |
        //            (uint)(auVar40._8_4_ * fStack_140) & uVar46) | (uint)(fVar25 * fStack_13c) & -(uint)(0.0 < trace));
        //out_lowerArm_gr->w =
        //    (float)(~- (uint)(0.0 < in_xmmTmp2_Dd) &
        //        (~uVar47 & (~- (uint)(m00 < in_xmmTmp2_Dd) & (uint)(fVar9 * local_148) |
        //            (uint)(fVar37 * fStack_144) & -(uint)(m00 < in_xmmTmp2_Dd)) |
        //            (uint)(fVar25 * fStack_140) & uVar47) |
        //        (uint)(auVar40._12_4_ * fStack_13c) & -(uint)(0.0 < in_xmmTmp2_Dd));
    }

	void RigArmDef::AnimateShoulder(Vector3* out_shoulder_rgp, Quat* out_shoulder_rgr, Vector3* out_upperArm_rgp, RigSkeleton* rigSkel, RigPoseUnit_Arm* poseUnit)
	{
        Vector3 spine_gp = rigSkel->GetGlobalParentPosition(ShoulderIndex);
        Quat spine_gr = rigSkel->GetGlobalParentRotation(ShoulderIndex);

        Vector3 shoulder_lp = rigSkel->GetLocalPosition(ShoulderIndex);

        *out_shoulder_rgp = Quat::rotate(spine_gr, shoulder_lp) + spine_gp;

        Quat shoulderRot = poseUnit->ShoulderRot;

        *out_shoulder_rgr = shoulderRot;

        Vector3 upperArm_lp = rigSkel->GetLocalPosition(UpperArmIndex);

        *out_upperArm_rgp = Quat::rotate(*out_shoulder_rgr, upperArm_lp) + *out_shoulder_rgp;
	}

	void RigArmDef::PoseToSkeleton(RigSkeleton* skeleton, RigPoseUnit_Arm* poseUnit)
	{
        Vector3 spine2eff_lv = poseUnit->Effector;

        Vector3 shoulder_gp = Vector3();
        Quat shoulder_gr = Quat::identity();
        Vector3 upperArm_rgp = Vector3();
        AnimateShoulder(&shoulder_gp, &shoulder_gr, &upperArm_rgp, skeleton, poseUnit);
        //if ((poseUnit->Flag & 0x10000) != 0) {
        //    FUN_141a89ab0(&shoulder_gr, &shoulder_gp, &upperArm_rgp, skeleton, poseUnit, (Vector3P*)&poseUnit->EffectorPos);
        //}

        Vector3 spine_gp = skeleton->GetGlobalParentPosition(ShoulderIndex);

        Quat chainRot = poseUnit->ChainRot;

        Vector3 effector_rgp = spine2eff_lv + spine_gp;

        Vector3 uarm2eff_rlv = effector_rgp - upperArm_rgp;

        Vector3 chain_uv = Vector3::normalize(Quat::rotate(chainRot, uarm2eff_rlv));

        Vector3 lowerArm_blp = skeleton->GetLocalPosition(LowerArmIndex);
        Vector3 effector_blp = skeleton->GetLocalPosition(EffectorSkelIndex);

        // Adjusts length of pab_v
        if ((skeleton->GetUnknownBool() == true) && ((poseUnit->Flag & 0x20000) != 0)) 
        {
            float lowerArm_blp_length = Vector3::length(lowerArm_blp);
            float effector_blp_length = Vector3::length(effector_blp);

            float straightArm_approxLength = (lowerArm_blp_length + effector_blp_length) - 0.01;

            float uarm2eff_length = Vector3::length(uarm2eff_rlv);

            if (straightArm_approxLength < uarm2eff_length)
            {
                float rescaleFactor = straightArm_approxLength / uarm2eff_length;
                Vector3 uarm2eff_rlv = uarm2eff_rlv * rescaleFactor;

                effector_rgp = pab_v_rescaled + upperArm_rgp;
            }
        }

        Quat upperArm_rgr = Quat::identity();
        Quat lowerArm_rgr = Quat::identity();
        RigRelatedTwoBoneArmMathFunc(&upperArm_rgr, &lowerArm_rgr, &lowerArm_blp, &effector_blp, &this->ChainPlaneNormal, &upperArm_rgp, &effector_rgp, &chain_uv);

        skeleton->SetGlobalRotation(ShoulderIndex, shoulder_gr);

        //// Bitfield
        //uVar1 = chainPoseIndexA >> 0x1f & 0x1f;
        //spineIndex = chainPoseIndexA + uVar1;
        //(&skeleton->Bitfield)[spineIndex >> 5] =
        //    (&skeleton->Bitfield)[spineIndex >> 5] | 1 << (((byte)spineIndex & 0x1f) - (char)uVar1 & 0x1f);

        skeleton->SetGlobalRotation(UpperArmIndex, upperArm_gr);

        //// Bitfield
        //uVar1 = chainPoseIndexBCpy >> 0x1f & 0x1f;
        //spineIndex = chainPoseIndexBCpy + uVar1;
        //(&skeleton->Bitfield)[spineIndex >> 5] =
        //    (&skeleton->Bitfield)[spineIndex >> 5] | 1 << (((byte)spineIndex & 0x1f) - (char)uVar1 & 0x1f);

        skeleton->SetGlobalRotation(LowerArmIndex, lowerArm_gr);

        //// Bitfield
        //uVar1 = lowerArmPoseIndex >> 0x1f & 0x1f;
        //spineIndex = lowerArmPoseIndex + uVar1;
        //(&skeleton->Bitfield)[spineIndex >> 5] =
        //    (&skeleton->Bitfield)[spineIndex >> 5] | 1 << (((byte)spineIndex & 0x1f) - (char)uVar1 & 0x1f);

        skeleton->SetGlobalPosition(ShoulderIndex, shoulder_gp);
        skeleton->SetGlobalPosition(UpperArmIndex, upperArm_rgp);


        Vector3 lowerArm_gp;
        eff_gp.w = (lowerArm_lp.z * upperArm_gr.y + lowerArm_lp.x * upperArm_gr.w) - lowerArm_lp.y * upperArm_gr.z;
        ur.x = (lowerArm_lp.x * upperArm_gr.z + lowerArm_lp.y * upperArm_gr.w) - lowerArm_lp.z * upperArm_gr.x;
        ur.y = (lowerArm_lp.y * upperArm_gr.x + lowerArm_lp.z * upperArm_gr.w) - lowerArm_lp.x * upperArm_gr.y;
        lowerArm_lp.w = (lowerArm_lp.w * upperArm_gr.w + lowerArm_lp.w * upperArm_gr.w) - lowerArm_lp.w * upperArm_gr.w;
        lowerArm_lp.y = lowerArm_lp.y * upperArm_gr.y + lowerArm_lp.x * upperArm_gr.x + lowerArm_lp.z * upperArm_gr.z;

        lowerArm_gp.y = ((lowerArm_lp.y * upperArm_gr.y + ur.x * upperArm_gr.w + eff_gp.w * upperArm_gr.z) - ur.y * upperArm_gr.x) + upperArm_rgp.y;
        lowerArm_gp.x = ((lowerArm_lp.y * upperArm_gr.x + eff_gp.w * upperArm_gr.w + ur.y * upperArm_gr.y) - ur.x * upperArm_gr.z) + upperArm_rgp.x;
        lowerArm_gp.z = ((lowerArm_lp.y * upperArm_gr.z + ur.y * upperArm_gr.w + ur.x * upperArm_gr.x) - eff_gp.w * upperArm_gr.y) + upperArm_rgp.z;
        lowerArm_gp.w = ((lowerArm_lp.y * upperArm_gr.w + lowerArm_lp.w * upperArm_gr.w + lowerArm_lp.w * upperArm_gr.w) - lowerArm_lp.w * upperArm_gr.w) + upperArm_rgp.w;
        skeleton->SetGlobalPosition(LowerArmIndex, lowerArm_gp);
	}
}