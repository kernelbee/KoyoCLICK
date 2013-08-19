#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "clickplc_ckp_nick.h"
#include "protocmn.h"

void parser_NICK(unsigned char* stream,int size,CLICKPLC_CKP_NICK* nick)
{
	int i_offset;
	int n_string;
	int i_item;
	
	char text[CLICKPLC_CKP_STRING_SIZE_MAX+1];
	
	i_offset = 0;
	
	memcpy(nick->header,stream + i_offset,CLICKPLC_CKP_NICK_HEADER_SIZE);
	i_offset += CLICKPLC_CKP_NICK_HEADER_SIZE;
	
	nick->n_items = stream[i_offset++];
	
	nick->items = (CKP_NICK_Item*)calloc(nick->n_items,sizeof(CKP_NICK_Item));
	
	printf_buf_hex_16(nick->header,CLICKPLC_CKP_NICK_HEADER_SIZE,"\nNICK header:\n");
	printf("NICK counter:\t%d\n",nick->n_items);	
	
	i_item = 0;
	n_string = 0;
	
	do 
	{
		int step;		
		int is_string;

		is_string = is_pascal_string(stream+i_offset,text,&step);

		if (is_string)
		{
			//printf("\"%s\"[%d: %02x]\t",text,step,text[0]);
			//printf("\"%s\"\t",text);

			printf("%24s\t",text);
			//printf_buf_hex(stream+i_offset,step,0);

			switch(n_string)
			{
			case 0:
				nick->items[i_item].pstr_register_name		= stream + i_offset;
				break;
			case 1:
				nick->items[i_item].pstr_register_number	= stream + i_offset;
				break;
			case 2:
				nick->items[i_item].pstr_nickname			= stream + i_offset;
				break;
			case 3:
				nick->items[i_item].pstr_initial_value		= stream + i_offset;				
				break;
			}
			
			n_string++;
			i_offset += step;			
		}
		else
		{				
			if(n_string)
			{
				if (stream[i_offset]==0)
				{
					switch(n_string)
					{
					case 0:
						nick->items[i_item].pstr_register_name		= 0;
						break;
					case 1:
						nick->items[i_item].pstr_register_number	= 0;
						break;
					case 2:
						nick->items[i_item].pstr_nickname			= 0;
						break;
					case 3:
						nick->items[i_item].pstr_initial_value		= 0;						
						break;
					}
					
					n_string++;
					printf("%24s\t","###################");

					//printf_buf_hex(stream + i_offset,1,0);
				}
				else
				{
					printf("\n");
					printf_buf_hex(stream + i_offset,1,0);
					n_string = 0;
				}
			}
			else
			{				
				memcpy(nick->items[i_item].subheader,stream + i_offset,CLICKPLC_CKP_NICK_SUBHEADERS_SIZE);

				printf_buf_hex(stream + i_offset,1,0);
				n_string = 0;
			}

			i_offset++;			
		}

		if (n_string==4)
		{			
			i_item++;

			if (i_item < nick->n_items)
			{
				int t_length;

				n_string = 0;
				printf("\n");

				t_length = (i_offset < size)? CLICKPLC_CKP_NICK_SUBHEADERS_SIZE : 0;
				if(t_length)
				{
					t_length = ( (i_offset+CLICKPLC_CKP_NICK_SUBHEADERS_SIZE) < size)? t_length : size-i_offset-CLICKPLC_CKP_NICK_SUBHEADERS_SIZE;
				}

				if (t_length)
				{				
					memcpy(nick->items[i_item].subheader,stream + i_offset,CLICKPLC_CKP_NICK_SUBHEADERS_SIZE);

					printf_buf_hex(stream + i_offset,t_length,0);
					i_offset += CLICKPLC_CKP_NICK_SUBHEADERS_SIZE;
				}
			}
		}

	} while (i_item < nick->n_items);

	memcpy(nick->footer,stream+size-CLICKPLC_CKP_NICK_FOOTER_SIZE,CLICKPLC_CKP_NICK_FOOTER_SIZE);

	printf("\n");
	printf_buf_hex(nick->footer,CLICKPLC_CKP_NICK_FOOTER_SIZE,"footer NICK:\n");
	printf("\n");

	return;
}

void destroy_NICK(CLICKPLC_CKP_NICK* nick)
{
	if (nick)
	{
		if (nick->items)
		{
			free(nick->items);
			nick->items = 0;
		}
		//free(nick);
	}

	return;
}


