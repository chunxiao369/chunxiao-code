From: "Leizhen (ThunderTown)" <thunder.leizhen@huawei.com>
To: Qu Wenruo <wqu@suse.com>,
	Linux Kernel Mailing List <linux-kernel@vger.kernel.org>,
	"linux-btrfs@vger.kernel.org" <linux-btrfs@vger.kernel.org>
Subject: Re: Please don't waste maintainers' time on your KPI grabbing patches (AKA, don't be a KPI jerk)
Date: Mon, 21 Jun 2021 18:56:23 +0800	[thread overview]
Message-ID: <0a9ae22c-44a0-6239-f61a-fa516f2a0de6@huawei.com> (raw)
In-Reply-To: <e78add0a-8211-86c3-7032-6d851c30f614@suse.com>

Hello, Qu:

My contributions to the kernel in the past have mainly been on optimizing the performance of the ARM64 SMMU driver,
including the iova optimization, strict mode optimization, and the lazy mode optimization. Also working on the
development of some ARM SoC drivers.

When time and effort is allowed, I also contribute to other modules of Linux kernel, trying to find something can be
improved, and some cleanup work is being done.

In the future, I will continue to make more and more important contributions to the Linux community.

Thanks.
Zhen

On 2021/6/18 14:31, Qu Wenruo wrote:
> Hi Leizhen, and guys in the mail list,
> 
> Recently I find one patch removing a debug OOM error message from btrfs selftest.
> 
> It's nothing special, some small cleanup work from some kernel newbie.
> 
> But the mail address makes me cautious, "@huawei.com".
> 
> The last time we got some similar patches from the same company, doing something harmless "cleanup". But those "fixes" are also useless.
> 
> This makes me wonder, what is really going on here.
> 
> After some quick search, more and more oom error message "cleanup" patches just show up, even some misspell fixes.
> 
> 
> It's OK for first-time/student developers to submit such patches, and I really hope such patches would make them become a long term contributor.
> In fact, I started my kernel contribution exactly by doing such "cleanups".
> 
> But what you guys are doing is really KPI grabbing, I have already see several maintainers arguing with you on such "cleanups", and you're always defending yourself to try to get those patches merged.
> 
> You're sending the patch representing your company, by doing this you're really just damaging the already broken reputation.
> 
> Please stop this KPI grabbing behavior, and do real contribution to fix the damaged reputation.
> 
> Thanks,
> Qu
> 
> 
> .
> 

next prev parent reply	other threads:[~2021-06-21 10:56 UTC|newest]

Thread overview: 5+ messages / expand[flat|nested]  mbox.gz  Atom feed  top
2021-06-18  6:31 Please don't waste maintainers' time on your KPI grabbing patches (AKA, don't be a KPI jerk) Qu Wenruo
2021-06-21 10:56 ` Leizhen (ThunderTown) [this message]
2021-06-21 11:40   ` Qu Wenruo
2021-06-21 15:28 ` Christoph Hellwig
2021-06-21 23:39   ` Qu Wenruo
Reply instructions:

You may reply publicly to this message via plain-text email
using any one of the following methods:

* Save the following mbox file, import it into your mail client,
  and reply-to-all from there: mbox

  Avoid top-posting and favor interleaved quoting:
  https://en.wikipedia.org/wiki/Posting_style#Interleaved_style

* Reply using the --to, --cc, and --in-reply-to
  switches of git-send-email(1):

  git send-email \
    --in-reply-to=0a9ae22c-44a0-6239-f61a-fa516f2a0de6@huawei.com \
    --to=thunder.leizhen@huawei.com \
    --cc=linux-btrfs@vger.kernel.org \
    --cc=linux-kernel@vger.kernel.org \
    --cc=wqu@suse.com \
    /path/to/YOUR_REPLY

  https://kernel.org/pub/software/scm/git/docs/git-send-email.html

* If your mail client supports setting the In-Reply-To header
  via mailto: links, try the mailto: link
Be sure your reply has a Subject: header at the top and a blank line before the message body

