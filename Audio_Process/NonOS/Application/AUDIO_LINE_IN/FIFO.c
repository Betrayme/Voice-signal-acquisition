//FIFO.c�ļ�

#include "FIFO.h"

/**
* @brief fifo�ṹ
*/

typedef struct
{
    long ptr_write;//д����
    long ptr_read;//��ȡ��
    bool is_full;//FIFO״̬

    long item_sum;// fifo�д洢��Ԫ����,�����ֽڴ�С
    int item_size;// Ԫ�ش�С.��λ: �ֽ�
    void *fifo_ptr; //FIFO��ַ
} Fifo;

/**
* @brief ����fifo
* @param item_sum��fifo��Ԫ����.ע�ⲻ���ֽ���
* @param item_size: Ԫ�ش�С.��λ: �ֽ�
* @return fifo����
*/

long fifo_create(long item_sum, int item_size)
{
    Fifo *fifo = (Fifo *)malloc(sizeof(Fifo));
    fifo->item_sum = item_sum;
    fifo->item_size = item_size;
    fifo->ptr_write = 0;
    fifo->ptr_read = 0;
    fifo->is_full = false;
    fifo->fifo_ptr = (void *)malloc(item_sum * item_size);
    return (long)fifo;
}
//item_sum<=327679
long fifosram_create(const long item_sum,int item_size)
{
    if(item_sum<=327679)
    {
        Fifo *fifo= (Fifo *)malloc(sizeof(Fifo));
        fifo->item_sum = item_sum;
        fifo->item_size = item_size;
        fifo->ptr_write = 0;
        fifo->ptr_read = 0;
        fifo->is_full = false;
        fifo->fifo_ptr = (void *)(Uint16 *)0x200000;
        return (long)fifo;
    }else
        return 0;
}

/**
* @brief ɾ��fifo
* @param fifo_index: fifo����
*/

void fifo_delete(long fifo_index)
{
    Fifo *fifo = (Fifo *)fifo_index;
    free(fifo->fifo_ptr);
    fifo->fifo_ptr = NULL;
    free(fifo);
    fifo = NULL;
}

/**
* @brief fifo����Ƿ����д��
* @param fifo_index: fifo����
* @retval false:������д��.true:����д��
*/

bool fifo_writeable(long fifo_index)
{
    Fifo *fifo = (Fifo *)fifo_index;
    return !fifo->is_full;
}

/**
* @brief fifoд��
* @param fifo_index: fifo����
* @param frame:д��Ԫ��ָ��
* @return false:ʧ��.true:�ɹ�
*/

bool fifo_write(long fifo_index, void *data)
{
  Fifo *fifo = (Fifo *)fifo_index;
  if (fifo->is_full)//�Ƿ���д��
    {
        return false;
    }
  /*memcpy((long*)(fifo->fifo_ptr) + fifo->ptr_write * fifo->item_size, (int*)data, fifo->item_size);*///�����ݸ��Ƶ�FIFO��
  *((long*)(fifo->fifo_ptr)+fifo->ptr_write * fifo->item_size)=*(int*)data;
  fifo->ptr_write++;//д����+1

  if (fifo->ptr_write >= fifo->item_sum)
  {
    fifo->is_full = true;//FIFO��
  }

    return true;
}

/**
* @brief fifo����д��
* @param fifo_index: fifo����
* @param data: д��Ԫ��ָ��
* @param item_num��д���Ԫ����Ŀ
* @return false:ʧ��.true:�ɹ�
*/

bool fifo_write_batch(long fifo_index, void *data, int item_num)
{
    Fifo *fifo = (Fifo *)fifo_index;
    if (fifo_writeable_item_count((long)fifo) < item_num)
    {
        return false;
    }

    memcpy((char *)(fifo->fifo_ptr) + fifo->ptr_write * fifo->item_size, data, fifo->item_size * item_num);
    fifo->ptr_write += item_num;
    if (fifo->ptr_write >= fifo->item_sum)
    {
        fifo->is_full = true;//FIFO��
    }
  return true;
}

/**
* @brief fifo����Ƿ���Զ�ȡ
* @param fifo_index: fifo����
* @return false:�����Զ�ȡ.true:���Զ�ȡ
*/

bool fifo_readable(long fifo_index)
{
  Fifo *fifo = (Fifo *)fifo_index;
  if (fifo->ptr_write == 0)//д����Ϊ0 ���ɶ�ȡ
    {
        return false;
    }
    return true;
}

/**
* @brief fifo��ȡ
* @param fifo_index: fifo����
* @param data: ��ȡ������
* @return false: ʧ��.true: �ɹ�
*/

bool fifo_read(long fifo_index, void *data)
{
  Fifo *fifo = (Fifo *)fifo_index;
  if (fifo->ptr_write == fifo->ptr_read && !fifo->is_full)
    {
        return false;
    }
    memcpy((int*)data, (long*)(fifo->fifo_ptr), fifo->item_size);
    fifo->ptr_read ++;
    //-----------------------------------------//
    memcpy((long*)(fifo->fifo_ptr),(long*)(fifo->fifo_ptr) + fifo->ptr_read * fifo->item_size,(fifo->item_sum - fifo->ptr_read));//�������������
    fifo->ptr_write -= fifo->ptr_read;//д������-=������������
    fifo->ptr_read = 0;//����������
    //========================================//
    fifo->is_full = false;
    return true;
}

/**
* @brief fifo������ȡ
* @param fifo_index: fifo����
* @param data: ��ȡ������
* @param item_num����ȡ��Ԫ����
* @return false: ʧ��.true: �ɹ�
*/

bool fifo_read_batch(long fifo_index, void *data, int item_num)
{
    Fifo *fifo = (Fifo *)fifo_index;
    if (fifo_readable_item_count((long)fifo) < item_num)
    {
        return false;
    }
    memcpy(data, (char *)(fifo->fifo_ptr), fifo->item_size * item_num);
    fifo->ptr_read += item_num;
    //-----------------------------------------//
    memcpy((char *)(fifo->fifo_ptr),(char *)(fifo->fifo_ptr) + fifo->ptr_read * fifo->item_size,(fifo->item_sum - fifo->ptr_read));//�������������
    fifo->ptr_write -= fifo->ptr_read;//д������-=������������
    fifo->ptr_read = 0;//����������
    //========================================//
    fifo->is_full = false;
    return true;
}

/**
* @brief fifo�ɶ���Ԫ����
* @param fifo_index: fifo����
* @return Ԫ����
*/

long fifo_readable_item_count(long fifo_index)
{
    Fifo *fifo = (Fifo *)fifo_index;
    if (fifo->is_full)
    {
        return fifo->item_sum;
    }
    else
    {
        return fifo->ptr_write;
    }
}

/**
* @brief fifo��д��Ԫ����
* @param fifo_index: fifo����
* @return Ԫ����
*/

long fifo_writeable_item_count(long fifo_index)
{
    Fifo *fifo = (Fifo *)fifo_index;
    if (fifo->is_full)
    {
        return 0;
    }
    else
    {
        return fifo->item_sum - fifo->ptr_write;
    }
}

