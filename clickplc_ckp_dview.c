#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "clickplc_ckp_dview.h"
#include "protocmn.h"

void parser_DVIEW(unsigned char* stream,int size,CLICKPLC_CKP_DVIEW* dview)
{
	int i_offset;
		
	i_offset = 0;
	
	memcpy(dview->header,stream + i_offset,CLICKPLC_CKP_DVIEW_HEADER_SIZE);
	i_offset += CLICKPLC_CKP_DVIEW_HEADER_SIZE;
				
	dview->n_cdv_files	= (unsigned short)stream[i_offset]|((unsigned short)stream[i_offset+1]<<8);
	i_offset += 2;
	
	//reading *.cdv files
	if (dview->n_cdv_files)
	{
		int x_file;
		int n_files;
		int n_bytes;

		n_files = dview->n_cdv_files;

		dview->cdv_files = (CLICKPLC_CKP_CDV_FILE*)calloc(n_files,sizeof(CLICKPLC_CKP_CDV_FILE));

		for (x_file = 0; x_file < n_files; x_file++)
		{
			if(is_pascal_string(stream + i_offset,dview->cdv_files[x_file].file_name,&n_bytes))
			{
				i_offset += n_bytes;
				
				dview->cdv_files[x_file].body_size = (unsigned short)stream[i_offset]|((unsigned short)stream[i_offset+1]<<8);
				dview->cdv_files[x_file].unknown_2x[0] = stream[i_offset+2];
				dview->cdv_files[x_file].unknown_2x[1] = stream[i_offset+3];
				
				i_offset += 4;

				if (dview->cdv_files[x_file].body_size > 0)
				{
					//including marker {ff fe}
					dview->cdv_files[x_file].body = (CKP_TEXT*)malloc(dview->cdv_files[x_file].body_size);
					memcpy(dview->cdv_files[x_file].body,stream + i_offset,dview->cdv_files[x_file].body_size);
					i_offset += dview->cdv_files[x_file].body_size;

					printf("\n%24s body(first 32 bytes):\n",dview->cdv_files[x_file].file_name);
					printf_buf_hex_16(dview->cdv_files[x_file].body,32,0);
				}
				else
				{
					//empty file
					dview->cdv_files[x_file].body = 0;
				}
			}
			else
			{
				//TODO!!!
				//error
				//printf("\n#%04x:\n",i_offset);
				//printf_buf_hex(stream+i_offset,64,"\n");
			}
		}		
	}
	dview->n_tv_files = 1;
	//reading *.tv files
	{
		int x_file;
		int n_files;
		int n_bytes;

		n_files = dview->n_tv_files;

		dview->tv_files = (CLICKPLC_CKP_TV_FILE*)calloc(n_files,sizeof(CLICKPLC_CKP_TV_FILE));

		for (x_file = 0; x_file < n_files; x_file++)
		{
			if(is_pascal_string(stream + i_offset,dview->tv_files[x_file].file_name,&n_bytes))
			{
				i_offset += n_bytes;

				dview->tv_files[x_file].body_size = (unsigned short)stream[i_offset]|((unsigned short)stream[i_offset+1]<<8);
				dview->tv_files[x_file].unknown_2x[0] = stream[i_offset+2];
				dview->tv_files[x_file].unknown_2x[1] = stream[i_offset+3];

				i_offset += 4;

				if (dview->tv_files[x_file].body_size > 0)
				{
					//including marker {ff fe}
					dview->tv_files[x_file].body = (CKP_TEXT*)malloc(dview->tv_files[x_file].body_size);
					memcpy(dview->tv_files[x_file].body,stream + i_offset,dview->tv_files[x_file].body_size);
					i_offset += dview->tv_files[x_file].body_size;

					printf("\n%24s body(first 32 bytes):\n",dview->tv_files[x_file].file_name);
					printf_buf_hex_16(dview->tv_files[x_file].body,32,0);
				}
				else
				{
					//empty file
					dview->tv_files[x_file].body = 0;
				}
			}
			else
			{
				//TODO!!!
				//error
			}
		}		
	}

	return;
}

#if 0
	i_item = 0;
	n_string = 0;

	do 
	{
		int step;		
		int is_string;

		is_string = is_pascal_string(stream+i_offset,text,&step);

		if (is_string)
		{
			printf("\n%s\n",text);
			//printf_buf_hex(stream+i_offset,step,0);
			
			n_string++;
			i_offset += step;			
		}
		else
		{		
			char ascii_ch1;
			char ascii_ch2;

			/*
			printf_buf_hex(stream + i_offset,1,0);
			n_string = 0;
			i_offset++;			
			*/

			ascii_ch1 = stream[i_offset+0];
			ascii_ch2 = stream[i_offset+1];

			if ((ascii_ch1>=0x1f) && (ascii_ch1<=0x7e) && (ascii_ch2==0))
			{
				printf("%c",ascii_ch1);
			}

			if ( ((ascii_ch1==0x0a) || (ascii_ch1==0x0d)) &&  (ascii_ch2==0))
			{
				printf("%c",ascii_ch1);
			}

			i_offset += 2;
		}
		
		if (is_string && n_string==1)
		{
			//printf_buf_hex_16(stream + i_offset,CLICKPLC_CKP_DVIEW_SUBHEADER_SIZE,"\nDVIEW subheader:\n");
			//i_offset += CLICKPLC_CKP_DVIEW_SUBHEADER_SIZE;

			printf("\n");
		}

	} while (i_offset < size);

#endif//0

void destroy_DVIEW(CLICKPLC_CKP_DVIEW* dview)
{
	if (dview)
	{
		int i,n;
		//*.cdv
		if (dview->cdv_files)
		{
			n = dview->n_cdv_files;
			for (i = 0; i < n; i++)
			{
				if (dview->cdv_files[i].body)
				{
					free(dview->cdv_files[i].body);
					dview->cdv_files[i].body = 0;
				}
			}

			free(dview->cdv_files);
			dview->cdv_files = 0;
		}
		//TextView.tv
		if (dview->tv_files)
		{
			n = dview->n_tv_files;
			for (i = 0; i < n; i++)
			{
				if (dview->tv_files[i].body)
				{
					free(dview->tv_files[i].body);
					dview->tv_files[i].body = 0;
				}
			}
			free(dview->tv_files);
			dview->tv_files = 0;
		}
		//free(dview);
	}

	return;
}