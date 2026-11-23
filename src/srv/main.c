#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/alloc.h>

#include <libresteam/srv/srv.h>

#define SRV_MAIN_EXIT_OK 0
#define SRV_MAIN_EXIT_FAIL 1

int main(void)
{
    libresteam_srv_t *srv = 0x0;
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t end_res = LDG_ERR_AOK;

    res = ldg_mem_init();
    if (res != LDG_ERR_AOK) { return SRV_MAIN_EXIT_FAIL; }

    res = ldg_mem_alloc(sizeof(libresteam_srv_t), &mem);
    if (res != LDG_ERR_AOK)
    {
        end_res = ldg_mem_shutdown();
        if (end_res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: mem shutdown failed"); }

        return SRV_MAIN_EXIT_FAIL;
    }

    srv = (libresteam_srv_t *)mem;

    res = libresteam_srv_init(srv);
    if (res == LDG_ERR_AOK) { res = libresteam_srv_run(srv); }

    if (res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_ERR, "srv: stopped with an error"); }

    end_res = libresteam_srv_shutdown(srv);
    if (end_res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: shutdown failed"); }

    end_res = ldg_mem_dealloc(mem);
    if (end_res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: dealloc failed"); }

    end_res = ldg_mem_shutdown();
    if (end_res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: mem shutdown failed"); }

    if (res != LDG_ERR_AOK) { return SRV_MAIN_EXIT_FAIL; }

    return SRV_MAIN_EXIT_OK;
}
