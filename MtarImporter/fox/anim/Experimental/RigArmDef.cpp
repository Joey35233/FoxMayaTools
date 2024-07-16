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

    void RigRelatedTwoBoneArmMathFunc(Quat* out_chainB_gr, Quat* out_chainC_gr, Vector3* chainC_lp, Vector3* effector_lp, Vector3* chainPlaneNormal, Vector3* chainB_gp, Vector3* effPlusGAP_gp, Vector3* chain_uv)
    {
        //float fVar1;
        //float fVar5;
        //float fVar6;
        //undefined auVar2[16];
        //undefined auVar3[16];
        //float fVar7;
        //undefined auVar4[16];
        //float fVar8;
        //float fVar11;
        //float fVar12;
        //undefined auVar9[16];
        //undefined auVar10[16];
        //float fVar13;
        //Vector3P p3rcpLen;
        //undefined auVar14[16];
        //undefined auVar15[16];
        //Vector4 auVar16;
        //undefined auVar17[16];
        //float fVar18;
        //float fVar20;
        //float fVar21;
        //undefined auVar19[16];
        //float fVar22;
        //float fVar23;
        //float fVar24;
        //float p3len2;
        //Vector4 auVar27;
        //undefined auVar25[16];
        //float fVar26;
        //float fVar27;
        //float fVar28;
        //float fVar29;
        //float fVar30;
        //Vector3P p3Nrm;
        //float fVar31;
        //float fVar32;
        //Vector4 fVar38;
        //undefined auVar33[16];
        //undefined auVar34[16];
        //uint uVar35;
        //uint uVar36;
        //uint uVar37;
        //uint uVar38;
        //float in_xmmTmp2_Dd;
        //uint local_158;
        //uint uStack_154;
        //uint uStack_150;
        //uint uStack_14c;
        //float local_148;
        //float fStack_144;
        //float fStack_140;
        //float fStack_13c;
        //float local_138;
        //float fStack_134;
        //float fStack_130;
        //float fStack_12c;
        //float local_128;
        //float fStack_124;
        //float fStack_120;
        //float fStack_11c;
        //float local_118;
        //float fStack_114;
        //float fStack_110;
        //float fStack_10c;
        //float local_108;
        //float fStack_104;
        //float fStack_100;
        //float fStack_fc;
        //undefined4 local_f8;
        //undefined4 uStack_f4;
        //undefined4 uStack_f0;
        //undefined4 uStack_ec;
        //undefined4 local_e8;
        //undefined4 uStack_e4;
        //undefined4 uStack_e0;
        //undefined4 uStack_dc;
        //undefined8 local_d8;
        //undefined4 uStack_d0;
        //undefined4 uStack_cc;
        //undefined8 local_c8;
        //undefined4 uStack_c0;
        //undefined4 uStack_bc;
        //ulonglong local_b8;

        //fVar8 = chainC_lp->x;
        //fVar11 = chainC_lp->y;
        //fVar12 = chainC_lp->z;
        //local_138 = effector_lp->x;
        //fStack_134 = effector_lp->y;
        //fStack_130 = effector_lp->z;
        //fVar38.x = k0.5.x;
        //fVar38.y = k0.5.y;
        //fVar38.w = k0.5.w;
        //local_128 = effPlusGAP_gp->x - chainB_gp->x;
        //fStack_124 = effPlusGAP_gp->y - chainB_gp->y;
        //fStack_120 = effPlusGAP_gp->z - chainB_gp->z;
        //fVar32 = effPlusGAP_gp->w - chainB_gp->w;
        //fVar1 = local_128 * local_128;
        //fVar5 = fStack_124 * fStack_124;
        //fVar6 = fStack_120 * fStack_120;
        //auVar16.x = fVar6 + fVar5 + fVar1;
        //auVar16.y = fVar6 + fVar5 + fVar1;
        //auVar16.z = fVar6 + fVar5 + fVar1;
        //auVar16.w = fVar6 + fVar5 + fVar1;
        //fVar13 = fStack_130 * fStack_130 + fStack_134 * fStack_134 + local_138 * local_138;
        //auVar17._0_4_ = fVar8 * fVar8;
        //auVar17._4_4_ = auVar17._0_4_;
        //auVar17._8_4_ = auVar17._0_4_;
        //auVar17._12_4_ = auVar17._0_4_;
        //p3len2 = fVar12 * fVar12 + fVar11 * fVar11 + auVar17._0_4_;
        //auVar27.y = auVar16.x;
        //auVar27.x = p3len2;
        //auVar27.z = auVar16.x;
        //auVar27.w = fVar13;
        //auVar27._8_8_ = auVar27._8_8_;
        //auVar27.y = fVar13;
        //auVar27.x = p3len2;
        //auVar2 = rsqrtps(auVar17, (undefined[16])auVar27);
        //auVar17 = sqrtps((undefined[16])auVar16, (undefined[16])auVar27);
        //fVar1 = auVar2._0_4_;
        //fVar5 = auVar2._4_4_;
        //fVar6 = auVar2._8_4_;
        //fVar31 = k0.5.z;
        //p3rcpLen.x = (3.0 - p3len2 * fVar1 * fVar1) * fVar38.x * fVar1;
        //p3rcpLen.y = (3.0 - fVar13 * fVar5 * fVar5) * fVar38.y * fVar5;
        //p3rcpLen.z = (3.0 - auVar16.x * fVar6 * fVar6) * fVar31 * fVar6;
        //p3Nrm.x = p3rcpLen.x * fVar8;
        //p3Nrm.y = p3rcpLen.x * fVar11;
        //p3Nrm.z = p3rcpLen.x * fVar12;
        //local_138 = p3rcpLen.y * local_138;
        //fStack_134 = p3rcpLen.y * fStack_134;
        //fStack_130 = p3rcpLen.y * fStack_130;
        //fStack_12c = p3rcpLen.y * effector_lp->w;
        //fVar8 = auVar17._0_4_;
        //fVar11 = auVar17._4_4_;
        //fVar12 = auVar17._8_4_;
        //fVar27 = p3rcpLen.z * local_128;
        //fVar28 = p3rcpLen.z * fStack_124;
        //fVar29 = p3rcpLen.z * fStack_120;
        //fVar30 = p3rcpLen.z * fVar32;
        //auVar19._0_4_ = ((p3len2 - fVar13) + auVar16.x) * fVar38.x * p3rcpLen.z;
        //auVar19._4_4_ = ((p3len2 - fVar13) + auVar16.x) * fVar38.y * p3rcpLen.z;
        //auVar19._8_4_ = ((p3len2 - fVar13) + auVar16.x) * fVar31 * p3rcpLen.z;
        //auVar19._12_4_ = ((p3len2 - fVar13) + auVar16.x) * fVar38.w * p3rcpLen.z;
        //auVar2 = maxps(auVar19, ZEXT816(0));
        //uVar35 = auVar2._0_4_;
        //uVar36 = -(uint)(fVar12 < fVar11 + fVar8);
        //uVar37 = -(uint)(fVar12 < fVar11 + fVar8);
        //uVar38 = -(uint)(fVar12 < fVar11 + fVar8);
        //fVar6 = (float)(uVar35 & uVar36 | ~uVar36 & (uint)fVar8);
        //fVar8 = (float)(uVar35 & uVar37 | ~uVar37 & (uint)fVar11);
        //fVar11 = (float)(uVar35 & uVar38 | ~uVar38 & (uint)fVar12);
        //fVar12 = (float)(uVar35 & -(uint)(fVar12 < in_xmmTmp2_Dd) | ~- (uint)(fVar12 < in_xmmTmp2_Dd) & auVar17._12_4_);
        //auVar2._0_4_ = fVar6 * fVar6;
        //auVar2._4_4_ = fVar8 * fVar8;
        //auVar2._8_4_ = fVar11 * fVar11;
        //auVar2._12_4_ = fVar12 * fVar12;
        //auVar25._0_4_ = p3len2 - auVar2._0_4_;
        //auVar25._4_4_ = fVar13 - auVar2._4_4_;
        //auVar25._8_4_ = auVar16.x - auVar2._8_4_;
        //auVar25._12_4_ = fVar13 - auVar2._12_4_;
        //auVar17 = maxps(auVar25, ZEXT816(0));
        //fVar8 = chain_uv->x;
        //fVar11 = chain_uv->y;
        //fVar12 = chain_uv->z;
        //fVar1 = chain_uv->w;
        //auVar2 = sqrtps(auVar2, auVar17);
        //fVar5 = auVar2._0_4_;
        //fVar18 = fVar6 * fVar27 + fVar5 * fVar8;
        //fVar20 = fVar6 * fVar28 + fVar5 * fVar11;
        //fVar21 = fVar6 * fVar29 + fVar5 * fVar12;
        //fVar22 = fVar6 * fVar30 + fVar5 * fVar1;
        //local_128 = local_128 - fVar18;
        //fStack_124 = fStack_124 - fVar20;
        //fStack_120 = fStack_120 - fVar21;
        //auVar3._0_4_ = fVar18 * fVar18;
        //fVar5 = fVar20 * fVar20;
        //fVar6 = fVar21 * fVar21;
        //auVar3._4_4_ = auVar3._0_4_;
        //auVar3._8_4_ = auVar3._0_4_;
        //auVar3._12_4_ = auVar3._0_4_;
        //auVar9._0_4_ = fVar6 + fVar5 + auVar3._0_4_;
        //auVar9._4_4_ = fVar6 + fVar5 + auVar3._0_4_;
        //auVar9._8_4_ = fVar6 + fVar5 + auVar3._0_4_;
        //auVar9._12_4_ = fVar6 + fVar5 + auVar3._0_4_;
        //auVar2 = rsqrtps(auVar3, auVar9);
        //fVar5 = auVar2._0_4_;
        //fVar6 = auVar2._4_4_;
        //p3len2 = auVar2._8_4_;
        //fVar23 = auVar2._12_4_;
        //auVar4._0_4_ = local_128 * local_128;
        //fVar13 = fStack_124 * fStack_124;
        //fVar7 = fStack_120 * fStack_120;
        //auVar4._4_4_ = auVar4._0_4_;
        //auVar4._8_4_ = auVar4._0_4_;
        //auVar4._12_4_ = auVar4._0_4_;
        //auVar10._0_4_ = fVar7 + fVar13 + auVar4._0_4_;
        //auVar10._4_4_ = fVar7 + fVar13 + auVar4._0_4_;
        //auVar10._8_4_ = fVar7 + fVar13 + auVar4._0_4_;
        //auVar10._12_4_ = fVar7 + fVar13 + auVar4._0_4_;
        //fVar18 = (3.0 - auVar9._0_4_ * fVar5 * fVar5) * fVar38.x * fVar5 * fVar18;
        //fVar20 = (3.0 - auVar9._4_4_ * fVar6 * fVar6) * fVar38.y * fVar6 * fVar20;
        //fVar21 = (3.0 - auVar9._8_4_ * p3len2 * p3len2) * fVar31 * p3len2 * fVar21;
        //fVar26 = (3.0 - auVar9._12_4_ * fVar23 * fVar23) * fVar38.w * fVar23 * fVar22;
        //fVar5 = chainPlaneNormal->x;
        //fVar6 = chainPlaneNormal->y;
        //fVar13 = chainPlaneNormal->z;
        //auVar2 = rsqrtps(auVar4, auVar10);
        //p3len2 = auVar2._0_4_;
        //fVar7 = auVar2._4_4_;
        //fVar23 = auVar2._8_4_;
        //fVar24 = auVar2._12_4_;
        //local_128 = (3.0 - auVar10._0_4_ * p3len2 * p3len2) * fVar38.x * p3len2 * local_128;
        //fStack_124 = (3.0 - auVar10._4_4_ * fVar7 * fVar7) * fVar38.y * fVar7 * fStack_124;
        //fStack_120 = (3.0 - auVar10._8_4_ * fVar23 * fVar23) * fVar31 * fVar23 * fStack_120;
        //fStack_11c = (3.0 - auVar10._12_4_ * fVar24 * fVar24) * fVar38.w * fVar24 * (fVar32 - fVar22);
        //fStack_110 = fVar11 * fVar29 - fVar12 * fVar28;
        //local_118 = fVar12 * fVar27 - fVar8 * fVar29;
        //fStack_114 = fVar8 * fVar28 - fVar11 * fVar27;
        //fStack_10c = fVar1 * fVar30 - fVar1 * fVar30;
        //fVar8 = fVar13 * p3Nrm.y - fVar6 * p3Nrm.z;
        //fVar11 = fVar5 * p3Nrm.z - fVar13 * p3Nrm.x;
        //fVar12 = fVar6 * p3Nrm.x - fVar5 * p3Nrm.y;
        //fVar1 = fVar20 * fStack_114 - fVar21 * local_118;
        //fVar23 = fVar21 * fStack_110 - fVar18 * fStack_114;
        //fVar24 = fVar18 * local_118 - fVar20 * fStack_110;
        //p3len2 = fVar5 * fStack_110 + p3Nrm.x * fVar18 + fVar8 * fVar1;
        //fVar7 = fVar5 * local_118 + p3Nrm.x * fVar20 + fVar8 * fVar23;
        //fVar5 = fVar5 * fStack_114 + p3Nrm.x * fVar21 + fVar8 * fVar24;
        //auVar14._0_4_ = fVar11 * fVar1;
        //auVar14._4_4_ = fVar11 * fVar23;
        //auVar14._8_4_ = fVar11 * fVar24;
        //auVar14._12_4_ = fVar11 * (fVar26 * fStack_10c - fVar26 * fStack_10c);
        //fVar22 = fVar6 * fStack_110 + p3Nrm.y * fVar18 + auVar14._0_4_;
        //fVar26 = fVar6 * local_118 + p3Nrm.y * fVar20 + auVar14._4_4_;
        //fVar27 = fVar6 * fStack_114 + p3Nrm.y * fVar21 + auVar14._8_4_;
        //fVar18 = fVar13 * fStack_110 + p3Nrm.z * fVar18 + fVar12 * fVar1;
        //fVar20 = fVar13 * local_118 + p3Nrm.z * fVar20 + fVar12 * fVar23;
        //fVar21 = fVar13 * fStack_114 + p3Nrm.z * fVar21 + fVar12 * fVar24;
        //fVar24 = fVar26 + p3len2 + fVar21;
        //auVar34._0_4_ = ((p3len2 - fVar26) - fVar21) + k1.0.x;
        //auVar34._4_4_ = ((fVar26 - fVar21) - p3len2) + k1.0.y;
        //auVar34._8_4_ = ((fVar21 - p3len2) - fVar26) + k1.0.z;
        //auVar34._12_4_ = fVar26 + p3len2 + fVar21 + k1.0.w;
        //auVar2 = rsqrtps(auVar14, auVar34);
        //fVar8 = auVar2._0_4_;
        //fVar11 = auVar2._4_4_;
        //fVar12 = auVar2._8_4_;
        //fVar1 = auVar2._12_4_;
        //fVar6 = fVar20 + fVar27;
        //fVar13 = fVar5 + fVar18;
        //fVar23 = fVar22 + fVar7;
        //fVar27 = fVar27 - fVar20;
        //fVar18 = fVar18 - fVar5;
        //fVar7 = fVar7 - fVar22;
        //fVar8 = (3.0 - fVar8 * auVar34._0_4_ * fVar8) * fVar8 * fVar38.x * fVar38.x;
        //fVar11 = (3.0 - fVar11 * auVar34._4_4_ * fVar11) * fVar11 * fVar38.y * fVar38.y;
        //fVar31 = (3.0 - fVar12 * auVar34._8_4_ * fVar12) * fVar12 * fVar31 * fVar31;
        //fVar12 = (3.0 - fVar1 * auVar34._12_4_ * fVar1) * fVar1 * fVar38.w * fVar38.w;
        //local_158 = -(uint)(fVar26 < fVar21 && p3len2 < fVar21);
        //uStack_154 = -(uint)(fVar26 < fVar21 && p3len2 < fVar21);
        //uStack_150 = -(uint)(fVar26 < fVar21 && p3len2 < fVar21);
        //uStack_14c = -(uint)(fVar26 < in_xmmTmp2_Dd && p3len2 < in_xmmTmp2_Dd);
        //out_chainB_gr->x =
        //    (float)(~- (uint)(0.0 < fVar24) &
        //        (~local_158 &
        //            (~- (uint)(p3len2 < fVar26) & (uint)(auVar34._0_4_ * fVar8) |
        //                (uint)(fVar23 * fVar11) & -(uint)(p3len2 < fVar26)) | (uint)(fVar13 * fVar31) & local_158) |
        //        (uint)(fVar27 * fVar12) & -(uint)(0.0 < fVar24));
        //out_chainB_gr->y =
        //    (float)(~- (uint)(0.0 < fVar24) &
        //        (~uStack_154 &
        //            (~- (uint)(p3len2 < fVar26) & (uint)(fVar23 * fVar8) |
        //                (uint)(auVar34._4_4_ * fVar11) & -(uint)(p3len2 < fVar26)) | (uint)(fVar6 * fVar31) & uStack_154) |
        //        (uint)(fVar18 * fVar12) & -(uint)(0.0 < fVar24));
        //out_chainB_gr->z =
        //    (float)(~- (uint)(0.0 < fVar24) &
        //        (~uStack_150 &
        //            (~- (uint)(p3len2 < fVar26) & (uint)(fVar13 * fVar8) | (uint)(fVar6 * fVar11) & -(uint)(p3len2 < fVar26))
        //            | (uint)(auVar34._8_4_ * fVar31) & uStack_150) | (uint)(fVar7 * fVar12) & -(uint)(0.0 < fVar24));
        //out_chainB_gr->w =
        //    (float)(~- (uint)(0.0 < in_xmmTmp2_Dd) &
        //        (~uStack_14c &
        //            (~- (uint)(p3len2 < in_xmmTmp2_Dd) & (uint)(fVar27 * fVar8) |
        //                (uint)(fVar18 * fVar11) & -(uint)(p3len2 < in_xmmTmp2_Dd)) | (uint)(fVar7 * fVar31) & uStack_14c) |
        //        (uint)(auVar34._12_4_ * fVar12) & -(uint)(0.0 < in_xmmTmp2_Dd));
        //fVar8 = chainPlaneNormal->x;
        //fVar11 = chainPlaneNormal->y;
        //fVar12 = chainPlaneNormal->z;
        //fVar1 = fVar12 * fStack_134 - fVar11 * fStack_130;
        //fVar5 = fVar8 * fStack_130 - fVar12 * local_138;
        //fVar6 = fVar11 * local_138 - fVar8 * fStack_134;
        //fVar13 = fStack_124 * fStack_114 - fStack_120 * local_118;
        //fVar31 = fStack_120 * fStack_110 - local_128 * fStack_114;
        //p3len2 = local_128 * local_118 - fStack_124 * fStack_110;
        //local_c8 = 0;
        //local_f8 = 0;
        //uStack_f4 = 0xffffffff;
        //uStack_f0 = 0;
        //uStack_ec = 0;
        //uStack_c0 = 0xffffffff;
        //uStack_bc = 0;
        //local_e8 = 0xffffffff;
        //uStack_e4 = 0;
        //uStack_e0 = 0;
        //uStack_dc = 0;
        //local_d8 = 0;
        //uStack_d0 = 0;
        //uStack_cc = 0xffffffff;
        //auVar15._0_4_ = fVar5 * fVar13;
        //auVar15._4_4_ = fVar5 * fVar31;
        //auVar15._8_4_ = fVar5 * p3len2;
        //auVar15._12_4_ = fVar5 * (fStack_11c * fStack_10c - fStack_11c * fStack_10c);
        //fVar20 = fVar8 * fStack_110 + local_138 * local_128 + fVar1 * fVar13;
        //fVar18 = fVar8 * local_118 + local_138 * fStack_124 + fVar1 * fVar31;
        //fVar21 = fVar8 * fStack_114 + local_138 * fStack_120 + fVar1 * p3len2;
        //fVar22 = fVar11 * fStack_110 + fStack_134 * local_128 + auVar15._0_4_;
        //fVar26 = fVar11 * local_118 + fStack_134 * fStack_124 + auVar15._4_4_;
        //fVar24 = fVar11 * fStack_114 + fStack_134 * fStack_120 + auVar15._8_4_;
        //fVar7 = fVar12 * fStack_110 + fStack_130 * local_128 + fVar6 * fVar13;
        //fVar23 = fVar12 * local_118 + fStack_130 * fStack_124 + fVar6 * fVar31;
        //fVar8 = fVar12 * fStack_114 + fStack_130 * fStack_120 + fVar6 * p3len2;
        //fVar31 = fVar26 + fVar20 + fVar8;
        //auVar33._0_4_ = ((fVar20 - fVar26) - fVar8) + k1.0.x;
        //auVar33._4_4_ = ((fVar26 - fVar8) - fVar20) + k1.0.y;
        //auVar33._8_4_ = ((fVar8 - fVar20) - fVar26) + k1.0.z;
        //auVar33._12_4_ = fVar26 + fVar20 + fVar8 + k1.0.w;
        //auVar2 = rsqrtps(auVar15, auVar33);
        //uVar35 = -(uint)(fVar26 < fVar8 && fVar20 < fVar8);
        //uVar36 = -(uint)(fVar26 < fVar8 && fVar20 < fVar8);
        //uVar37 = -(uint)(fVar26 < fVar8 && fVar20 < fVar8);
        //uVar38 = -(uint)(fVar26 < in_xmmTmp2_Dd && fVar20 < in_xmmTmp2_Dd);
        //fVar8 = auVar2._0_4_;
        //fVar11 = auVar2._4_4_;
        //fVar12 = auVar2._8_4_;
        //fVar1 = auVar2._12_4_;
        //fVar5 = fVar23 + fVar24;
        //fVar6 = fVar21 + fVar7;
        //fVar13 = fVar22 + fVar18;
        //fVar24 = fVar24 - fVar23;
        //fVar7 = fVar7 - fVar21;
        //fVar18 = fVar18 - fVar22;
        //local_148 = (3.0 - fVar8 * auVar33._0_4_ * fVar8) * k0.5.x * fVar8 * k0.5.x;
        //fStack_144 = (3.0 - fVar11 * auVar33._4_4_ * fVar11) * k0.5.y * fVar11 * k0.5.y;
        //fStack_140 = (3.0 - fVar12 * auVar33._8_4_ * fVar12) * k0.5.z * fVar12 * k0.5.z;
        //fStack_13c = (3.0 - fVar1 * auVar33._12_4_ * fVar1) * k0.5.w * fVar1 * k0.5.w;
        //out_chainC_gr->x =
        //    (float)(~- (uint)(0.0 < fVar31) &
        //        (~uVar35 & (~- (uint)(fVar20 < fVar26) & (uint)(auVar33._0_4_ * local_148) |
        //            (uint)(fVar13 * fStack_144) & -(uint)(fVar20 < fVar26)) | (uint)(fVar6 * fStack_140) & uVar35)
        //        | (uint)(fVar24 * fStack_13c) & -(uint)(0.0 < fVar31));
        //out_chainC_gr->y =
        //    (float)(~- (uint)(0.0 < fVar31) &
        //        (~uVar36 & (~- (uint)(fVar20 < fVar26) & (uint)(fVar13 * local_148) |
        //            (uint)(auVar33._4_4_ * fStack_144) & -(uint)(fVar20 < fVar26)) |
        //            (uint)(fVar5 * fStack_140) & uVar36) | (uint)(fVar7 * fStack_13c) & -(uint)(0.0 < fVar31));
        //out_chainC_gr->z =
        //    (float)(~- (uint)(0.0 < fVar31) &
        //        (~uVar37 & (~- (uint)(fVar20 < fVar26) & (uint)(fVar6 * local_148) |
        //            (uint)(fVar5 * fStack_144) & -(uint)(fVar20 < fVar26)) |
        //            (uint)(auVar33._8_4_ * fStack_140) & uVar37) | (uint)(fVar18 * fStack_13c) & -(uint)(0.0 < fVar31));
        //out_chainC_gr->w =
        //    (float)(~- (uint)(0.0 < in_xmmTmp2_Dd) &
        //        (~uVar38 & (~- (uint)(fVar20 < in_xmmTmp2_Dd) & (uint)(fVar24 * local_148) |
        //            (uint)(fVar7 * fStack_144) & -(uint)(fVar20 < in_xmmTmp2_Dd)) |
        //            (uint)(fVar18 * fStack_140) & uVar38) |
        //        (uint)(auVar33._12_4_ * fStack_13c) & -(uint)(0.0 < in_xmmTmp2_Dd));
        //local_108 = fStack_114;
        //fStack_104 = fStack_110;
        //fStack_100 = local_118;
        //fStack_fc = fStack_10c;
    }

	void RigArmDef::AnimateShoulder(Vector3* out_chainA_gp, Quat* out_chainA_gr, Vector3* out_chainB_gp, RigSkeleton* rigSkel, RigPoseUnit_Arm* poseUnit)
	{
        Vector3 ap_gp = rigSkel->GetGlobalParentPosition(ChainIndexA);
        Quat ap_gr = rigSkel->GetGlobalParentRotation(ChainIndexA);

        Vector3 a_lp = rigSkel->GetLocalPosition(ChainIndexA);

        *out_chainA_gp = Quat::rotate(ap_gr, a_lp) + ap_gp;

        Quat shoulderRot = poseUnit->ShoulderRot;

        *out_chainA_gr = shoulderRot;

        Vector3 b_lp = rigSkel->GetLocalPosition(ChainIndexB);

        *out_chainB_gp = Quat::rotate(*out_chainA_gr, b_lp) + *out_chainA_gp;
	}

	void RigArmDef::PoseToSkeleton(RigSkeleton* skeleton, RigPoseUnit_Arm* poseUnit)
	{
        Vector3 eff_gp = poseUnit->EffectorPos;

        Vector3 chainA_gp = Vector3();
        Quat chainA_gr = Quat::identity();
        Vector3 chainB_gp = Vector3();
        AnimateShoulder(&chainA_gp, &chainA_gr, &chainB_gp, skeleton, poseUnit);
        //if ((poseUnit->Flag & 0x10000) != 0) {
        //    FUN_141a89ab0(&chainA_gr, &chainA_gp, &chainB_gp, skeleton, poseUnit, (Vector3P*)&poseUnit->EffectorPos);
        //}

        Vector3 chainAParent_gp = skeleton->GetGlobalParentPosition(ChainIndexA);

        Quat ur = poseUnit->UnknownRot;

        Vector3 effPlusCAP_gp = eff_gp + chainAParent_gp;

        Vector3 pab_v = effPlusCAP_gp - chainB_gp;

        Vector3 chain_uv = Vector3::normalize(Quat::rotate(ur, pab_v));

        Vector3 chainC_lp = skeleton->GetLocalPosition(ChainIndexC);
        Vector3 effector_lp = skeleton->GetLocalPosition(EffectorSkelIndex);

        // Adjusts length of pab_v
        if ((skeleton->GetUnknownBool() == true) && ((poseUnit->Flag & 0x20000) != 0)) 
        {
            float chainC_lp_length = Vector3::length(chainC_lp);
            float effector_lp_length = Vector3::length(effector_lp);

            float straightArm_approxLength = (chainC_lp_length + effector_lp_length) - 0.01;

            float pab_v_length = Vector3::length(pab_v);

            if (straightArm_approxLength < pab_v_length)
            {
                float rescaleFactor = straightArm_approxLength / pab_v_length;
                Vector3 pab_v_rescaled = pab_v * rescaleFactor;

                effPlusCAP_gp = pab_v_rescaled + chainB_gp;
            }
        }

        Quat chainB_gr = Quat::identity();
        Quat chainC_gr = Quat::identity();
        RigRelatedTwoBoneArmMathFunc(&chainB_gr, &chainC_gr, &chainC_lp, &effector_lp, &this->ChainPlaneNormal, &chainB_gp, &effPlusCAP_gp, &chain_uv);

        skeleton->SetGlobalRotation(ChainIndexA, chainA_gr);

        //// Bitfield
        //uVar1 = chainPoseIndexA >> 0x1f & 0x1f;
        //chainAParentIndex = chainPoseIndexA + uVar1;
        //(&skeleton->Bitfield)[chainAParentIndex >> 5] =
        //    (&skeleton->Bitfield)[chainAParentIndex >> 5] | 1 << (((byte)chainAParentIndex & 0x1f) - (char)uVar1 & 0x1f);

        skeleton->SetGlobalRotation(ChainIndexB, chainB_gr);

        //// Bitfield
        //uVar1 = chainPoseIndexBCpy >> 0x1f & 0x1f;
        //chainAParentIndex = chainPoseIndexBCpy + uVar1;
        //(&skeleton->Bitfield)[chainAParentIndex >> 5] =
        //    (&skeleton->Bitfield)[chainAParentIndex >> 5] | 1 << (((byte)chainAParentIndex & 0x1f) - (char)uVar1 & 0x1f);

        skeleton->SetGlobalRotation(ChainIndexC, chainC_gr);

        //// Bitfield
        //uVar1 = chainCPoseIndex >> 0x1f & 0x1f;
        //chainAParentIndex = chainCPoseIndex + uVar1;
        //(&skeleton->Bitfield)[chainAParentIndex >> 5] =
        //    (&skeleton->Bitfield)[chainAParentIndex >> 5] | 1 << (((byte)chainAParentIndex & 0x1f) - (char)uVar1 & 0x1f);

        skeleton->SetGlobalPosition(ChainIndexA, chainA_gp);
        skeleton->SetGlobalPosition(ChainIndexB, chainB_gp);


        Vector3 chainC_gp;
        eff_gp.w = (chainC_lp.z * chainB_gr.y + chainC_lp.x * chainB_gr.w) - chainC_lp.y * chainB_gr.z;
        ur.x = (chainC_lp.x * chainB_gr.z + chainC_lp.y * chainB_gr.w) - chainC_lp.z * chainB_gr.x;
        ur.y = (chainC_lp.y * chainB_gr.x + chainC_lp.z * chainB_gr.w) - chainC_lp.x * chainB_gr.y;
        chainC_lp.w = (chainC_lp.w * chainB_gr.w + chainC_lp.w * chainB_gr.w) - chainC_lp.w * chainB_gr.w;
        chainC_lp.y = chainC_lp.y * chainB_gr.y + chainC_lp.x * chainB_gr.x + chainC_lp.z * chainB_gr.z;

        chainC_gp.y = ((chainC_lp.y * chainB_gr.y + ur.x * chainB_gr.w + eff_gp.w * chainB_gr.z) - ur.y * chainB_gr.x) + chainB_gp.y;
        chainC_gp.x = ((chainC_lp.y * chainB_gr.x + eff_gp.w * chainB_gr.w + ur.y * chainB_gr.y) - ur.x * chainB_gr.z) + chainB_gp.x;
        chainC_gp.z = ((chainC_lp.y * chainB_gr.z + ur.y * chainB_gr.w + ur.x * chainB_gr.x) - eff_gp.w * chainB_gr.y) + chainB_gp.z;
        chainC_gp.w = ((chainC_lp.y * chainB_gr.w + chainC_lp.w * chainB_gr.w + chainC_lp.w * chainB_gr.w) - chainC_lp.w * chainB_gr.w) + chainB_gp.w;
        skeleton->SetGlobalPosition(ChainIndexC, chainC_gp);
	}
}