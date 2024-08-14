//FIFO.h�ļ�

#ifndef _FIFO_H_
#define _FIFO_H_

#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdbool.h"
#include "string.h"
#include "math.h"
#include "DSP2833x_Device.h"

/**
* @brief ����fifo
* @param item_sum��fifo��Ԫ����.ע�ⲻ���ֽ���
* @param item_size: Ԫ�ش�С.��λ: �ֽ�
* @return fifo����
*/

extern long fifo_create(long item_sum, int item_size);

extern long fifosram_create(const long item_sum,int item_size);

/**
* @brief ɾ��fifo
* @param fifo_index: fifo����
*/

extern void fifo_delete(long fifo_index);

/**
* @brief fifo����Ƿ����д��
* @param fifo_index: fifo����
* @retval false:������д��.true:����д��
*/

extern bool fifo_writeable(long fifo_index);

/**
* @brief fifoд��
* @param fifo_index: fifo����
* @param frame:д��Ԫ��ָ��
* @return false:ʧ��.true:�ɹ�
*/

extern bool fifo_write(long fifo_index, void *data);

/**
* @brief fifo����д��
* @param fifo_index: fifo����
* @param data: д��Ԫ��ָ��
* @param @param item_num��д���Ԫ����Ŀ
* @return false:ʧ��.true:�ɹ�
*/

extern bool fifo_write_batch(long fifo_index, void *data, int item_num);

/**
* @brief fifo����Ƿ���Զ�ȡ
* @param fifo_index: fifo����
* @return false:�����Զ�ȡ.true:���Զ�ȡ
*/

extern bool fifo_readable(long fifo_index);

/**
* @brief fifo��ȡ
* @param fifo_index: fifo����
* @param data: ��ȡ������
* @return false: ʧ��.true: �ɹ�
*/

extern bool fifo_read(long fifo_index, void *data);

/**
* @brief fifo������ȡ
* @param fifo_index: fifo����
* @param data: ��ȡ������
* @param item_num����ȡ��Ԫ����
* @return false: ʧ��.true: �ɹ�
*/

extern bool fifo_read_batch(long fifo_index, void *data, int item_num);

/**
* @brief fifo�ɶ���Ԫ����
* @param fifo_index: fifo����
* @return Ԫ����
*/

extern long fifo_readable_item_count(long fifo_index);

/**
* @brief fifo��д��Ԫ����
* @param fifo_index: fifo����
* @return Ԫ����
*/

extern long fifo_writeable_item_count(long fifo_index);


extern void TestFifo(void);
#endif
