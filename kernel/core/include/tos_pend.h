/*----------------------------------------------------------------------------
 * Tencent is pleased to support the open source community by making TencentOS
 * available.
 *
 * Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
 * If you have downloaded a copy of the TencentOS binary from Tencent, please
 * note that the TencentOS binary is licensed under the BSD 3-Clause License.
 *
 * If you have downloaded a copy of the TencentOS source code from Tencent,
 * please note that TencentOS source code is licensed under the BSD 3-Clause
 * License, except for the third-party components listed below which are
 * subject to different license terms. Your integration of TencentOS into your
 * own projects may require compliance with the BSD 3-Clause License, as well
 * as the other licenses applicable to the third-party components included
 * within TencentOS.
 *---------------------------------------------------------------------------*/

#ifndef _TOS_PEND_H_
#define  _TOS_PEND_H_

typedef struct k_task_st k_task_t;

/**
 * The reason why we wakeup from a pend.
 * when we wakeup, we need to know why.
 */
typedef enum pend_state_en {
    PEND_STATE_NONE,        /**< nothing. */
    PEND_STATE_POST,        /**< someone has post, we get what we want. */
    PEND_STATE_TIMEOUT,     /**< a post has never came until time is out. */
    PEND_STATE_DESTROY,      /**< someone has destroyed what we are pending for. */
    PEND_STATE_OWNER_DIE,   /**< the pend object owner task is destroyed. */
} pend_state_t;

// what we are pending
/* actually, it's some kind of magic number, mainly for identifing whether the pend
   is initialized, or whether user pass the correct parameter.
 */
typedef enum pend_type_en {
    PEND_TYPE_NONE              = 0x0000,
    PEND_TYPE_SEM               = 0x1BEE,
    PEND_TYPE_MUTEX             = 0x2BEE,
    PEND_TYPE_EVENT             = 0x3BEE,
    PEND_TYPE_QUEUE             = 0x4BEE,
    PEND_TYPE_COUNTDOWNLATCH    = 0x5BEE,
    PEND_TYPE_COMPLETION        = 0x6BEE,
} pend_type_t;

typedef enum opt_post_en {
    OPT_POST_ONE,
    OPT_POST_ALL,
} opt_post_t;

typedef struct pend_object_st {
    pend_type_t     type;
    k_list_t        list;
} pend_obj_t;

__KERNEL__ void     pend_object_init(pend_obj_t *object, pend_type_t type);

__KERNEL__ void     pend_object_deinit(pend_obj_t *object);

__KERNEL__ int      pend_object_verify(pend_obj_t *object, pend_type_t type);

__KERNEL__ int      pend_is_nopending(pend_obj_t *object);

__KERNEL__ k_prio_t pend_highest_pending_prio_get(pend_obj_t *object);

__KERNEL__ k_task_t *pend_highest_pending_task_get(pend_obj_t *object);

__KERNEL__ void     pend_list_remove(k_task_t *task);

__KERNEL__ void     pend_list_adjust(k_task_t *task);

__KERNEL__ k_err_t  pend_state2errno(pend_state_t state);

__KERNEL__ void     pend_task_wakeup(k_task_t *task, pend_state_t state);

__KERNEL__ void     pend_task_block(k_task_t *task, pend_obj_t *object, k_tick_t timeout);

__KERNEL__ void     pend_wakeup_one(pend_obj_t *object, pend_state_t state);

__KERNEL__ void     pend_wakeup_all(pend_obj_t *object, pend_state_t state);

__KERNEL__ void     pend_wakeup(pend_obj_t *object, pend_state_t state, opt_post_t opt);

#endif /* _TOS_PEND_H_ */

