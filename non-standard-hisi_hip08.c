/*
 * Copyright (c) 2017 Hisilicon Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ras-record.h"
#include "ras-logger.h"
#include "ras-report.h"
#include "ras-non-standard-handler.h"

/* common definitions */

/* HISI SAS definitions */
#define HISI_SAS_VALID_PA           BIT(0)
#define HISI_SAS_VALID_MB_ERR       BIT(1)
#define HISI_SAS_VALID_NFE_TYPE     BIT(2)
#define HISI_SAS_VALID_FE_TYPE      BIT(3)
#define HISI_SAS_VALID_ECC_EC	    BIT(4)
#define HISI_SAS_VALID_HGC_ECC      BIT(5)

struct hisi_sas_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   physical_addr;
	uint32_t   mb;
	uint64_t   nfe_type;
	uint32_t   fe_type;
	uint64_t   ecc_ec;
	uint64_t   hgc_ec;
};

/* HISI HNS definitions */
#define HISI_HNS_VALID_ERR_TYPE       BIT(0)
#define HISI_HNS_VALID_UE_ERR         BIT(1)
#define HISI_HNS_VALID_PORT_ID        BIT(2)

struct hisi_hns_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   type;
	uint32_t   ue;
	uint32_t   port_id;
};

/* HISI ROCEE definitions */
#define HISI_ROCEE_VALID_ERR_TYPE       BIT(0)
#define HISI_ROCEE_VALID_MB_ERR         BIT(1)

struct hisi_rocee_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   type;
	uint32_t   mb;
};

/* HISI PCIe definitions */
#define HISI_PCIE_VALID_ERR_TYPE       BIT(0)
#define HISI_PCIE_VALID_MB_ERR         BIT(1)

struct hisi_pcie_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   type;
	uint32_t   mb;
};

/* HISI SEC definitions */
#define HISI_SEC_VALID_ERR_ADDR       BIT(0)
#define HISI_SEC_VALID_ERR_TYPE       BIT(1)
#define HISI_SEC_VALID_ECC_EC         BIT(2)

struct hisi_sec_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   mem_addr;
	uint64_t   type;
	uint64_t   ecc_ec;
};

/* HISI ZIP definitions */
#define HISI_ZIP_VALID_ERR_ADDR       BIT(0)
#define HISI_ZIP_VALID_ERR_TYPE       BIT(1)
#define HISI_ZIP_VALID_ECC_EC         BIT(2)

struct hisi_zip_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   mem_addr;
	uint64_t   type;
	uint64_t   ecc_ec;
};

/* HISI SMMU definitions */
#define HISI_SMMU_VALID_ERR_ADDR       BIT(0)
#define HISI_SMMU_VALID_NON_SECURED    BIT(1)
#define HISI_SMMU_VALID_ERR_TYPE       BIT(2)
#define HISI_SMMU_VALID_UE             BIT(3)
#define HISI_SMMU_VALID_OVF            BIT(4)
#define HISI_SMMU_VALID_TP_ECC_EC      BIT(5)
#define HISI_SMMU_VALID_SP_ECC_EC      BIT(6)
#define HISI_SMMU_VALID_CACHE_ECC_EC   BIT(7)
#define HISI_SMMU_VALID_L2_TLB_ECC_EC  BIT(8)

struct hisi_smmu_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   mem_addr;
	uint64_t   ns_attr;
	uint64_t   type;
	uint16_t   ue;
	uint16_t   ovf;
	uint64_t   tp_ecc_ec;
	uint64_t   sp_ecc_ec;
	uint64_t   cntxt_cache_ecc_ec;
	uint64_t   l2_tlb_ecc_ec;
};

/* HISI HHA definitions */
#define HISI_HHA_VALID_ERR_ADDR       BIT(0)
#define HISI_HHA_VALID_MB_ERR         BIT(1)
#define HISI_HHA_VALID_ERR_TYPE       BIT(2)
#define HISI_HHA_VALID_UE             BIT(3)
#define HISI_HHA_VALID_OVF            BIT(4)
#define HISI_HHA_VALID_1B_ECC_EC      BIT(5)

struct hisi_hha_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   mem_addr;
	uint32_t   mb;
	uint64_t   type;
	uint16_t   ue;
	uint16_t   ovf;
	uint64_t   sbit_ecc_ec;
};

/* HISI HLLC definitions */
#define HISI_HLLC_VALID_ERR_ADDR       BIT(0)
#define HISI_HLLC_VALID_NON_SECURED    BIT(1)
#define HISI_HLLC_VALID_ERR_TYPE       BIT(2)
#define HISI_HLLC_VALID_ERR_CODE       BIT(3)
#define HISI_HLLC_VALID_UE             BIT(4)
#define HISI_HLLC_VALID_OVF            BIT(5)

struct hisi_hllc_err_sec {
	uint64_t   val_bits;
	uint64_t   soc_id;
	uint32_t   version;
	uint64_t   mem_addr;
	uint64_t   ns_attr;
	uint64_t   type;
	uint64_t   err_code;
	uint16_t   ue;
	uint16_t   ovf;
};

static int decode_hip08_sas_error(struct trace_seq *s, const void *error);
static int decode_hip08_hns_error(struct trace_seq *s, const void *error);
static int decode_hip08_rocee_error(struct trace_seq *s, const void *error);
static int decode_hip08_pcie_error(struct trace_seq *s, const void *error);
static int decode_hip08_sec_error(struct trace_seq *s, const void *error);
static int decode_hip08_zip_error(struct trace_seq *s, const void *error);
static int decode_hip08_smmu_error(struct trace_seq *s, const void *error);
static int decode_hip08_hha_error(struct trace_seq *s, const void *error);
static int decode_hip08_hllc_error(struct trace_seq *s, const void *error);

static struct ras_ns_dec_tab hisi_ns_dec_tab[] = {
	{
		.sec_type = "daffd8146eba4d8c8a91bc9bbf4aa301",
		.decode = decode_hip08_sas_error,
	},
	{
		.sec_type = "fbc2d923ea7a453dab132949f5af9e53",
		.decode = decode_hip08_hns_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_rocee_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_pcie_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_sec_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_zip_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_smmu_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_hha_error,
	},
	{
		.sec_type = "00000000000000000000000000000000",
		.decode = decode_hip08_hllc_error,
	},
};

/* Common Functions */
static char *err_bit_type(int etype)
{
	switch (etype) {
	case 0x0: return "single-bit ecc";
	case 0x1: return "multi-bit ecc";
	}
	return "unknown error";
}

static char *err_ue_type(int etype)
{
	switch (etype) {
	case 0x0: return "CE";
	case 0x1: return "UE";
	}
	return "unknown error";
}

static char *secured_addr_type(int addr_type)
{
	switch (addr_type) {
	case 0x0: return "SECURED ADDRESS";
	case 0x1: return "NON SECURED ADDRESS";
	}
	return "unknown addr type";
}

static char *err_ovf_type(int ovf)
{
	switch (ovf) {
	case 0x0: return "OVERFLOW";
	case 0x1: return "NO OVERFLOW";
	}
	return "unknown OVF status";
}

/* SAS Functions */
static char *sas_nfe_type(uint64_t etype)
{
	switch (etype) {
	case BIT_ULL(0): return "hgc_dqe_1b_ecc";
	case BIT_ULL(1): return "hgc_dqe_2b_ecc";
	case BIT_ULL(2): return "hgc_iost_1b_ecc";
	case BIT_ULL(3): return "hgc_iost_2b_ecc";
	case BIT_ULL(4): return "hgc_itct_1b_ecc";
	case BIT_ULL(5): return "hgc_itct_2b_ecc";
	case BIT_ULL(6): return "hgc_iostl_1b_ecc";
	case BIT_ULL(7): return "hgc_iostl_2b_ecc";
	case BIT_ULL(8): return "hgc_itctl_1b_ecc";
	case BIT_ULL(9): return "hgc_itctl_2b_ecc";
	case BIT_ULL(10): return "hgc_cqe_1b_ecc";
	case BIT_ULL(11): return "hgc_cqe_2b_ecc";
	case BIT_ULL(12): return "rxm_cfg_mem0_1b_ecc";
	case BIT_ULL(13): return "rxm_cfg_mem0_2b_ecc";
	case BIT_ULL(14): return "rxm_cfg_mem1_1b_ecc";
	case BIT_ULL(15): return "rxm_cfg_mem1_2b_ecc";
	case BIT_ULL(16): return "rxm_cfg_mem2_1b_ecc";
	case BIT_ULL(17): return "rxm_cfg_mem2_2b_ecc";
	case BIT_ULL(18): return "rxm_cfg_mem3_1b_ecc";
	case BIT_ULL(19): return "rxm_cfg_mem3_2b_ecc";
	case BIT_ULL(20): return "DMAC0_ram_1b_ecc";
	case BIT_ULL(21): return "DMAC0_ram_2b_ecc";
	case BIT_ULL(22): return "DMAC1_ram_1b_ecc";
	case BIT_ULL(23): return "DMAC1_ram_2b_ecc";
	case BIT_ULL(24): return "DMAC2_ram_1b_ecc";
	case BIT_ULL(25): return "DMAC2_ram_2b_ecc";
	case BIT_ULL(26): return "DMAC3_ram_1b_ecc";
	case BIT_ULL(27): return "DMAC3_ram_2b_ecc";
	case BIT_ULL(28): return "DMAC4_ram_1b_ecc";
	case BIT_ULL(29): return "DMAC4_ram_2b_ecc";
	case BIT_ULL(30): return "DMAC5_ram_1b_ecc";
	case BIT_ULL(31): return "DMAC5_ram_2b_ecc";
	case BIT_ULL(32): return "DMAC6_ram_1b_ecc";
	case BIT_ULL(33): return "DMAC6_ram_2b_ecc";
	case BIT_ULL(34): return "DMAC7_ram_1b_ecc";
	case BIT_ULL(35): return "DMAC7_ram_2b_ecc";
	case BIT_ULL(36): return "OOO_ram_1b_ecc";
	case BIT_ULL(37): return "OOO_ram_2b_ecc";
	case BIT_ULL(38): return "HLINK";
	case BIT_ULL(39): return "HLINK_PLL0_out_of_lock";
	case BIT_ULL(40): return "HLINK_PLL1_out_of_lock";
	case BIT_ULL(41): return "HLINK_loss_of_refclk0";
	case BIT_ULL(42): return "HLINK_loss_of_refclk1";
	case BIT_ULL(43): return "DMAC0_TX_poison";
	case BIT_ULL(44): return "DMAC1_TX_poison";
	case BIT_ULL(45): return "DMAC2_TX_poison";
	case BIT_ULL(46): return "DMAC3_TX_poison";
	case BIT_ULL(47): return "DMAC4_TX_poison";
	case BIT_ULL(48): return "DMAC5_TX_poison";
	case BIT_ULL(49): return "DMAC6_TX_poison";
	case BIT_ULL(50): return "DMAC7_TX_poison";
	case BIT_ULL(51): return "hgc_dqe_poison";
	case BIT_ULL(52): return "hgc_iost_poison";
	case BIT_ULL(53): return "hgc_itct_poison";
	case BIT_ULL(54): return "hgc_itct_ncq_poison";
	case BIT_ULL(55): return "DMAC0_RX_poison";
	case BIT_ULL(56): return "DMAC1_RX_poison";
	case BIT_ULL(57): return "DMAC2_RX_poison";
	case BIT_ULL(58): return "DMAC3_RX_poison";
	case BIT_ULL(59): return "DMAC4_RX_poison";
	case BIT_ULL(60): return "DMAC5_RX_poison";
	case BIT_ULL(61): return "DMAC6_RX_poison";
	case BIT_ULL(62): return "DMAC7_RX_poison";
	}
	return "unknown error";
}

static char *sas_fe_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "iost_axi_wr_err";
	case BIT(1): return "iost_axi_rd_err";
	case BIT(2): return "itct_axi_wr_err";
	case BIT(3): return "itct_axi_rd_err";
	case BIT(4): return "sata_axi_wr_err";
	case BIT(5): return "sata_axi_rd_err";
	case BIT(6): return "dqe_axi_rd_err";
	case BIT(7): return "cqe_axi_wr_err";
	case BIT(8): return "cqe_winfo_fifo";
	case BIT(9): return "cqe_msg_fifo";
	case BIT(10): return "getdqe_fifo";
	case BIT(11): return "cmdp_fifo";
	case BIT(12): return "awtctrl_fifo";
	}
	return "unknown error";
}

static int decode_hip08_sas_error(struct trace_seq *s,
				  const void *error)
{
	char buf[1536];
	char *p = buf;
	const struct hisi_sas_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n", __func__);
		return -1;
	}
	p += sprintf(p, "[");
	if (err->val_bits & HISI_SAS_VALID_PA)
		p += sprintf(p, "phy addr = 0x%p: ",
				(void *)err->physical_addr);

	if (err->val_bits & HISI_SAS_VALID_MB_ERR)
		p += sprintf(p, "%s: ", err_bit_type(err->mb));

	if (err->val_bits & HISI_SAS_VALID_NFE_TYPE)
		p += sprintf(p, "non fatal error type = %s: ",
				sas_nfe_type(err->nfe_type));
	if (err->val_bits & HISI_SAS_VALID_FE_TYPE)
		p += sprintf(p, "fatal error type = %s: ",
				sas_fe_type(err->fe_type));
	if (err->val_bits & HISI_SAS_VALID_ECC_EC)
		p += sprintf(p, "ECC error count = %llu : ",
					(unsigned long long)err->ecc_ec);
	if (err->val_bits & HISI_SAS_VALID_FE_TYPE)
		p += sprintf(p, "HGC error count = %llu : ",
					(unsigned long long)err->hgc_ec);
	p += sprintf(p, "]");

	trace_seq_printf(s, "\nHISI HIP08: SAS error: %s\n", buf);
	return 0;
}

/* HNS Functions */
static char *hns_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "linear_ram_1bit_ecc";
	case BIT(1): return "linear_ram_2bit_ecc";
	case BIT(2): return "tcam_ram_1bit_ecc";
	case BIT(3): return "tcam_ram_2bit_ecc";
	}
	return "unknown error";
}

static int decode_hip08_hns_error(struct trace_seq *s,  const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_hns_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_HNS_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ", hns_err_type(err->type));

	/*if (err->val_bits & HISI_HNS_VALID_UE_ERR)
	 *	p += sprintf(p, "%s: ", err_ue_type(err->ue));
	 */
	if (err->val_bits & HISI_HNS_VALID_PORT_ID)
		p += sprintf(p, "port id = %d", err->port_id);

	p += sprintf(p, "]");

	trace_seq_printf(s, "\nHISI HIP08: HNS error: %s\n", buf);
	return 0;
}


/* ROCEE Functions */
static char *rocee_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "qmm_internal_cache_ecc";
	case BIT(1): return "trp_internal_mem_1b_ecc";
	case BIT(2): return "tsp_internal_mem_1b_ecc";
	case BIT(3): return "qmm_internal_mem_1b_ecc";
	case BIT(4): return "trp_internal_mem_2b_ecc";
	case BIT(5): return "mdb_internal_mem_2b_ecc";
	case BIT(6): return "caep_internal_mem_2b_ecc";
	case BIT(7): return "tm_internal_mem_2b_ecc";
	case BIT(8): return "tsp_internal_mem_2b_ecc";
	case BIT(9): return "qmm_internal_mem_2b_ecc";
	}
	return "unknown error";
}

static int decode_hip08_rocee_error(struct trace_seq *s,
				    const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_rocee_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_ROCEE_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ", rocee_err_type(err->type));

	if (err->val_bits & HISI_ROCEE_VALID_MB_ERR)
		p += sprintf(p, "%s: ", err_bit_type(err->mb));

	p += sprintf(p, "]");
	trace_seq_printf(s, "\nHISI HIP08: ROCEE error: %s\n", buf);
	return 0;
}

/* PCIe Functions */
static char *pcie_err_type(uint64_t etype)
{
	switch (etype) {
	case 0: return "unknown error";
	}
	return "unknown error";
}

static int decode_hip08_pcie_error(struct trace_seq *s,
				   const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_pcie_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_PCIE_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ", pcie_err_type(err->type));

	if (err->val_bits & HISI_PCIE_VALID_MB_ERR)
		p += sprintf(p, "%s: ", err_bit_type(err->mb));

	p += sprintf(p, "]");

	trace_seq_printf(s, "\nHISI HIP08: PCIe error: %s\n", buf);
	return 0;
}

/* SEC Functions */
static char *sec_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "q_bd_fail_sat";
	case BIT(1): return "q_ivld_sat";
	case BIT(2): return "q_wr_ptr_overflow";
	case BIT(3): return "q_outorder_rd_ptr_overflow";
	case BIT(4): return "q_rd_err";
	case BIT(5): return "q_ecc_err";
	case BIT(6): return "q_bd_mac_wr_err";
	case BIT(7): return "q_err_bd_wr_err";
	case BIT(8): return "q_bd_wr_err";
	case BIT(9): return "cpl_ecc_err";
	case BIT(10): return "cpl_berr";
	case BIT(11): return "cpl_q_full_bd_q_not_empty";
	case BIT(12): return "cpl_q_read_full_err";
	}
	return "unknown error";
}

static int decode_hip08_sec_error(struct trace_seq *s,
				  const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_sec_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_SEC_VALID_ERR_ADDR)
		p += sprintf(p, "mem addr = 0x%p: ",
			     (void *)err->mem_addr);

	if (err->val_bits & HISI_SEC_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ", sec_err_type(err->type));

	if (err->val_bits & HISI_SEC_VALID_ECC_EC)
		p += sprintf(p, "ECC EC = %llu: ",
			     (unsigned long long)err->ecc_ec);
	p += sprintf(p, "]");
	trace_seq_printf(s, "\nHISI HIP08: SEC error: %s\n", buf);
	return 0;
}

/* ZIP Functions */
static char *zip_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "1b_ecc_err";
	case BIT(1): return "2b_ecc_err";
	case BIT(2): return "axi_rresp_err";
	case BIT(3): return "axi_bresp_err";
	case BIT(4): return "src_addr_parse";
	case BIT(5): return "dst_addr_parse";
	case BIT(6): return "pre_in_addr";
	case BIT(7): return "pre_in_data";
	case BIT(8): return "com_inf";
	case BIT(9): return "enc_inf";
	case BIT(10): return "pre_out";
	}
	return "unknown error";
}

static int decode_hip08_zip_error(struct trace_seq *s,
				  const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_zip_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_ZIP_VALID_ERR_ADDR)
		p += sprintf(p, "mem addr = 0x%p: ",
			     (void *)err->mem_addr);

	if (err->val_bits & HISI_ZIP_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ",
			     zip_err_type(err->type));

	if (err->val_bits & HISI_ZIP_VALID_ECC_EC)
		p += sprintf(p, "1bit ECC EC = %lu: ",
			     err->ecc_ec & 0xFFFF);
		p += sprintf(p, "2bit ECC EC = %lu: ",
			     (err->ecc_ec >> 16));
	p += sprintf(p, "]");

	trace_seq_printf(s, "\nHISI HIP08: ZIP error: %s\n", buf);
	return 0;
}

/* SMMU Functions */
static char *smmu_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "sfe_fetch";
	case BIT(1): return "cd_fetch";
	case BIT(2): return "walk_eabt";
	case BIT(3): return "cmdq_fetch";
	case BIT(4): return "wr_eventq_abort";
	case BIT(5): return "cntxt_cache_ecc";
	case BIT(6): return "l2_tlb_ecc";
	case BIT(7): return "wrbuff_ecc";
	}
	return "unknown error";
}

static int decode_hip08_smmu_error(struct trace_seq *s,
				   const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_smmu_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_SMMU_VALID_ERR_ADDR)
		p += sprintf(p, "mem addr = 0x%p: ",
			     (void *)err->mem_addr);

	if (err->val_bits & HISI_SMMU_VALID_NON_SECURED)
		p += sprintf(p, "%s: ", secured_addr_type(err->ns_attr));

	if (err->val_bits & HISI_SMMU_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ", smmu_err_type(err->type));

	if (err->val_bits & HISI_SMMU_VALID_UE)
		p += sprintf(p, "%s: ", err_ue_type(err->ue));

	if (err->val_bits & HISI_SMMU_VALID_OVF)
		p += sprintf(p, "%s: ", err_ovf_type(err->ovf));

	if (err->val_bits & HISI_SMMU_VALID_TP_ECC_EC)
		p += sprintf(p, "TP Port WrBuff ECC EC = %llu: ",
			     (unsigned long long)err->tp_ecc_ec);

	if (err->val_bits & HISI_SMMU_VALID_SP_ECC_EC)
		p += sprintf(p, "SP Port WrBuff ECC EC = %llu: ",
			     (unsigned long long)err->sp_ecc_ec);

	if (err->val_bits & HISI_SMMU_VALID_CACHE_ECC_EC)
		p += sprintf(p, "Context cache ECC EC = %llu: ",
			     (unsigned long long)err->cntxt_cache_ecc_ec);

	if (err->val_bits & HISI_SMMU_VALID_L2_TLB_ECC_EC)
		p += sprintf(p, "L2 TLB ECC EC = %llu: ",
			     (unsigned long long)(err->l2_tlb_ecc_ec));
	p += sprintf(p, "]");
	trace_seq_printf(s, "\nHISI HIP08: SMMU error: %s\n", buf);
	return 0;
}

/* HHA Functions */
static char *hha_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT_ULL(0): return "illegal_opcode";
	case BIT_ULL(1): return "cmd_buf_mb_ecc";
	case BIT_ULL(2): return "sdir_mb_ecc";
	case BIT_ULL(3): return "edir_mb_ecc";
	case BIT_ULL(4): return "default_slave";
	case BIT_ULL(5): return "sec_zone_access_err";
	case BIT_ULL(6): return "data_buf_mb_ecc";
	case BIT_ULL(7): return "ddrc_resp_err_uer";
	case BIT_ULL(8): return "msd_miss";
	case BIT_ULL(9): return "msd_overlap";
	case BIT_ULL(10): return "msd_invert";
	case BIT_ULL(11): return "ns_sec_zone_access_err";
	case BIT_ULL(12): return "ns_sec_zone_access_err";
	case BIT_ULL(13): return "count_overflow_1b_ecc";
	case BIT_ULL(14): return "cmd_buf_1b_ecc";
	case BIT_ULL(15): return "data_buf_1b_ecc";
	case BIT_ULL(16): return "s_dir_1b_ecc";
	case BIT_ULL(17): return "e_dir_1b_ecc";
	case BIT_ULL(18): return "mem_1b_ecc";
	case BIT_ULL(19): return "buf_mem_mb_ecc";
	case BIT_ULL(20): return "dir_mem_mb_ecc";
	case BIT_ULL(21): return "address_err";
	case BIT_ULL(22): return "illegal_request";
	case BIT_ULL(23): return "ddrc_resp_err";
	}
	return "unknown error";
}

static int decode_hip08_hha_error(struct trace_seq *s,
				  const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_hha_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_HHA_VALID_ERR_ADDR)
		p += sprintf(p, "mem addr = 0x%p: ",
			     (void *)err->mem_addr);

	if (err->val_bits & HISI_HHA_VALID_MB_ERR)
		p += sprintf(p, "%s: ", err_bit_type(err->mb));

	if (err->val_bits & HISI_HHA_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ", hha_err_type(err->type));

	if (err->val_bits & HISI_HHA_VALID_UE)
		p += sprintf(p, "%s: ", err_ue_type(err->ue));

	if (err->val_bits & HISI_HHA_VALID_OVF)
		p += sprintf(p, "%s: ", err_ovf_type(err->ovf));

	if (err->val_bits & HISI_HHA_VALID_1B_ECC_EC)
		p += sprintf(p, "1B ECC EC = %llu: ",
			     (unsigned long long)err->sbit_ecc_ec);

	p += sprintf(p, "]");
	trace_seq_printf(s, "\nHISI HIP08: HHA error: %s\n", buf);
	return 0;
}

/* HLLC Functions */
static char *hllc_err_type(uint64_t etype)
{
	switch (etype) {
	case BIT(0): return "1b_mem_err";
	case BIT(1): return "crc_err";
	case BIT(2): return "fatal_err";
	case BIT(3): return "hydra_pcs_fatal_err";
	case BIT(4): return "hydra_pcs_non_fatal_err";
	case BIT(5): return "non_mem_ecc_err";
	case BIT(6): return "mem_ecc_err";
	}
	return "unknown error";
}

static char *hllc_err_code(uint64_t etype)
{
	switch (etype) {
	case BIT_ULL(0): return "hydra_tx_ch0_2bit_ecc";
	case BIT_ULL(1): return "hydra_tx_ch1_2bit_ecc";
	case BIT_ULL(2): return "hydra_tx_ch2_2bit_ecc";
	case BIT_ULL(3): return "phy_tx_retry_2bit_ecc";
	case BIT_ULL(4): return "hydra_rx_ch0_2bit_ecc";
	case BIT_ULL(5): return "hydra_rx_ch1_2bit_ecc";
	case BIT_ULL(6): return "hydra_rx_ch2_2bit_ecc";
	case BIT_ULL(7): return "phy_rx_retry_ptr_err";
	case BIT_ULL(8): return "phy_tx_retry_buf_ptr_err";
	case BIT_ULL(9): return "phy_tx_retry_ptr_err";
	case BIT_ULL(10): return "hydra_tx_ch0_ovf";
	case BIT_ULL(11): return "hydra_tx_ch1_ovf";
	case BIT_ULL(12): return "hydra_tx_ch2_ovf";
	case BIT_ULL(13): return "phy_tx_retry_buf_ovf";
	case BIT_ULL(14): return "hydra_rx_ch0_ovf";
	case BIT_ULL(15): return "hydra_rx_ch1_ovf";
	case BIT_ULL(16): return "hydra_rx_ch2_ovf";
	case BIT_ULL(17): return "hydra_pcs_err0";
	case BIT_ULL(18): return "hydra_pcs_err1";
	case BIT_ULL(19): return "hydra_pcs_err2";
	case BIT_ULL(20): return "hydra_pcs_err3";
	case BIT_ULL(21): return "hydra_pcs_err4";
	case BIT_ULL(22): return "hydra_pcs_err5";
	case BIT_ULL(23): return "hydra_pcs_err6";
	case BIT_ULL(24): return "hydra_pcs_err7";
	case BIT_ULL(25): return "hydra_tx_ch0_1bit_ecc";
	case BIT_ULL(26): return "hydra_tx_ch1_1bit_ecc";
	case BIT_ULL(27): return "hydra_tx_ch2_1bit_ecc";
	case BIT_ULL(28): return "phy_tx_retry_1bit_ecc";
	case BIT_ULL(29): return "hydra_rx_ch0_1bit_ecc";
	case BIT_ULL(30): return "hydra_rx_ch1_1bit_ecc";
	case BIT_ULL(31): return "hydra_rx_ch2_1bit_ecc";
	case BIT_ULL(32): return "phy_rx_flit_crc";
	}
	return "unknown error";
}

static int decode_hip08_hllc_error(struct trace_seq *s,
				   const void *error)
{
	static char buf[1024];
	char *p = buf;
	const struct hisi_hllc_err_sec *err = error;

	if (err->val_bits == 0) {
		trace_seq_printf(s, "\n%s: no valid error data\n",
				 __func__);
		return -1;
	}

	p += sprintf(p, "[");
	if (err->val_bits & HISI_HLLC_VALID_ERR_ADDR)
		p += sprintf(p, "mem addr = 0x%p: ",
			     (void *)err->mem_addr);

	if (err->val_bits & HISI_HLLC_VALID_NON_SECURED)
		p += sprintf(p, "%s: ", secured_addr_type(err->ns_attr));

	if (err->val_bits & HISI_HLLC_VALID_ERR_TYPE)
		p += sprintf(p, "error type = %s: ",
			     hllc_err_type(err->type));

	if (err->val_bits & HISI_HLLC_VALID_ERR_CODE)
		p += sprintf(p, "error code = %s: ",
			     hllc_err_code(err->err_code));

	if (err->val_bits & HISI_HLLC_VALID_UE)
		p += sprintf(p, "%s: ", err_ue_type(err->ue));

	if (err->val_bits & HISI_HLLC_VALID_OVF)
		p += sprintf(p, "%s: ", err_ovf_type(err->ovf));

	p += sprintf(p, "]");
	trace_seq_printf(s, "\nHISI HIP08: HLLC error: %s\n", buf);
	return 0;
}

__attribute__((constructor))
static void hip08_init(void)
{
	hisi_ns_dec_tab[0].len = ARRAY_SIZE(hisi_ns_dec_tab);
	register_ns_dec_tab(hisi_ns_dec_tab);
}

